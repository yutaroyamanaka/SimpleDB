/* Copyright 2021 Yutaro Yamanaka */
#include "logrecord.hpp"
#include "page.hpp"

namespace tx {
  std::unique_ptr<LogRecord> LogRecord::createLogRecord(const std::vector<char>& bytes) {
    auto share_bytes = std::make_shared<std::vector<char>>(bytes);
    file::Page p(share_bytes);

    switch (p.getInt(0)) {
      case CHECKPOINT:
        return std::make_unique<CheckpointRecord>();
      case START:
        return std::make_unique<StartRecord>(&p);
      case COMMIT:
        return std::make_unique<CommitRecord>(&p);
      case ROLLBACK:
        return std::make_unique<RollbackRecord>(&p);
      case SETINT:
        return std::make_unique<SetIntRecord>(&p);
      case SETSTRING:
        return std::make_unique<SetStringRecord>(&p);
      default:
        return std::unique_ptr<LogRecord>(nullptr);
    }
  }

  CheckpointRecord::CheckpointRecord() {
  }

  int CheckpointRecord::writeToLog(log::LogManager* lm) {
    auto bytes = std::make_shared<std::vector<char>>(sizeof(uint32_t), 0);
    file::Page p(bytes);
    p.setInt(0, CHECKPOINT);
    return lm->append(*bytes);
  }

  StartRecord::StartRecord(file::Page* page) {
    int tpos = sizeof(uint32_t);
    txnum_ = page->getInt(tpos);
  }

  int StartRecord::writeToLog(log::LogManager* lm, int txnum) {
    int tpos = sizeof(uint32_t);
    int reclen = tpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    file::Page p(bytes);
    p.setInt(0, START);
    p.setInt(tpos, txnum);
    return lm->append(*bytes);
  }

  CommitRecord::CommitRecord(file::Page* page) {
    int tpos = sizeof(uint32_t);
    txnum_ = page->getInt(tpos);
  }

  int CommitRecord::writeToLog(log::LogManager* lm, int txnum) {
    int tpos = sizeof(uint32_t);
    int reclen = tpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    file::Page p(bytes);
    p.setInt(0, COMMIT);
    p.setInt(tpos, txnum);
    return lm->append(*bytes);
  }

  RollbackRecord::RollbackRecord(file::Page* p) {
    int pos = sizeof(uint32_t);
    txnum_ = p->getInt(pos);
  }

  int RollbackRecord::writeToLog(log::LogManager* lm, int txnum) {
    int tpos = sizeof(uint32_t);
    int reclen = tpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    file::Page p(bytes);
    p.setInt(0, ROLLBACK);
    p.setInt(tpos, txnum);
    return lm->append(*bytes);
  }

  SetIntRecord::SetIntRecord(file::Page* p) {
    int tpos = sizeof(uint32_t);
    txnum_ = p->getInt(tpos);
    int fpos = tpos + sizeof(uint32_t);
    std::string fileName = p->getString(fpos);
    int bpos = fpos + file::Page::maxLength(fileName.size());
    int blknum = p->getInt(bpos);
    block_id_ = file::BlockId(fileName, blknum);
    int opos = bpos + sizeof(uint32_t);
    offset_ = p->getInt(opos);
    int vpos = opos + sizeof(uint32_t);
    val_ = p->getInt(vpos);
  }

  std::string SetIntRecord::toString() {
    return "<SETINT, " + std::to_string(txnum_) + ", " + block_id_.toString()  + ", " + std::to_string(offset_) + ", " + std::to_string(val_) + ">";
  }

  void SetIntRecord::undo(Transaction* tx) {
    tx->pin(block_id_);
    tx->setInt(block_id_, offset_, val_, false);
    tx->unpin(block_id_);
  }

  int SetIntRecord::writeToLog(log::LogManager* lm, int txnum, file::BlockId& block_id, int offset, int val) {
    int tpos = sizeof(uint32_t);
    int fpos = tpos + sizeof(uint32_t);
    int bpos = fpos + file::Page::maxLength(block_id.fileName().size());
    int opos = bpos + sizeof(uint32_t);
    int vpos = opos + sizeof(uint32_t);
    int reclen = vpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    file::Page p(bytes);
    p.setInt(0, SETINT);
    p.setInt(tpos, txnum);
    p.setString(fpos, block_id.fileName());
    p.setInt(bpos, block_id.number());
    p.setInt(opos, offset);
    p.setInt(vpos, val);
    return lm->append(*bytes);
  }

  SetStringRecord::SetStringRecord(file::Page* p) {
    int tpos = sizeof(uint32_t);
    txnum_ = p->getInt(tpos);
    int fpos = tpos + sizeof(uint32_t);
    std::string filename = p->getString(fpos);
    int bpos = fpos + file::Page::maxLength(filename.size());
    int blknum = p->getInt(bpos);
    block_id_ = file::BlockId(filename, blknum);
    int opos = bpos + sizeof(uint32_t);
    offset_ = p->getInt(opos);
    int vpos = opos + sizeof(uint32_t);
    val_ = p->getString(vpos);
  }

  std::string SetStringRecord::toString() {
    return "<SETSTRING " + std::to_string(txnum_) + ", "  + block_id_.toString() + ", " + std::to_string(offset_) + ", " + val_ + ">";
  }

  void SetStringRecord::undo(Transaction* tx) {
    tx->pin(block_id_);
    tx->setString(block_id_, offset_, val_, false);  // don't log the undo
    tx->unpin(block_id_);
  }

  int SetStringRecord::writeToLog(log::LogManager* lm, int txnum, file::BlockId& block_id, int offset, std::string val) {
    int tpos = sizeof(uint32_t);
    int fpos = tpos + sizeof(uint32_t);
    int bpos = fpos + file::Page::maxLength(block_id.fileName().size());
    int opos = bpos + sizeof(uint32_t);
    int vpos = opos + sizeof(uint32_t);
    int reclen = vpos + file::Page::maxLength(val.size());
    auto rec = std::make_shared<std::vector<char>>(reclen, 0);
    file::Page p(rec);
    p.setInt(0, SETSTRING);
    p.setInt(tpos, txnum);
    p.setString(fpos, block_id.fileName());
    p.setInt(bpos, block_id.number());
    p.setInt(opos, offset);
    p.setString(vpos, val);
    return lm->append(*rec);
  }
}  // namespace tx
