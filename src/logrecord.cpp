#pragma once
#include "logrecord.hpp"
#include "page.hpp"
#include <memory.h>

namespace tx {
  LogRecord::~LogRecord() {

  }

  std::make_unique<LogRecord> LogRecord::createLogRecord(std::shared_ptr<std::vector<char>>& bytes) {
    auto p = std::make_shared<file::Page>(bytes);
    switch(p->getInt(0)) {
      case CHECKPOINT:
        return std::make_unique<CheckpointRecord>();
      case START:
        return std::make_unique<StartRecord>(p);
      case COMMIT:
        return std::make_unique<CommitRecord>(p);
      case ROLLBACK:
        return std::make_unique<RollbackRecord>(p);
      case SETINT:
        return std::make_unique<SetIntRecord>(p);
      case SETSTRING:
        return std::make_unique<SetStringRecord>(p);
      default:
        return nullptr;
    }
  }

  CheckpointRecord::CheckpointRecord() {

  }

  static int CheckpointRecord::writeToLog(log::LogManager* lm) {
    auto bytes = std::make_shared<std::vector<char>>(sizeof(uint32_t), 0);
    auto p = std::make_unique<file::Page>(bytes);
    p->setInt(0, CHECKPOINT);
    return lm->append(*bytes);
  }

  StartRecord::StartRecord(file::Page* page) {
    int tpos = sizeof(uint32_t);
    txnum_ = page->getInt(tpos);
  }

  static int StartRecord::writeToLog(log::LogManager* lm, int txnum) {
    int tpos = sizeof(uint32_t);
    int reclen = tpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    auto p = std::make_unique<file::Page>(bytes);
    p->setInt(0, START);
    p->setInt(tpos, txnum_);
    return lm->append(*bytes);
  }

  CommitRecord::CommitRecord(file::Page* page) {
    int tpos = sizeof(uint32_t);
    txnum_ = page->getInt(tpos);
  }

  static int CommitRecord::writeToLog(log::LogManager* lm, int txnum) {
    int tpos = sizeof(uint32_t);
    int reclen = tpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    auto p = std::make_unique<file::Page>(bytes);
    p->setInt(0, COMMIT);
    p->setInt(tpos, txnum_);
    return lm->append(*bytes);
  }

  RollbackRecord::RollbackRecord(file::Page* p) {
    int pos = sizeof(uint32_t);
    txnum_ = p->getInt(pos);
  }

  static int RollbackRecord::writeToLog(log::LogManager* lm, int txnum) {
    int tpos = sizeof(uint32_t);
    int reclen = tpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    auto p = std::make_unique<file::Page>(bytes);
    p->setInt(0, ROLLBACK);
    p->setInt(tpos, txnum_);
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

  void SetIntRecord::toString() {
    return "<SETINT, " + std::to_string(txnum_) + ", " + block_id_.toString()  + ", " + std::to_string(offset_) + ", " + std::to_string(val_) + ">";
  }

  void SetIntRecord::undo(Transaction* tx) {
    tx->pin(block_id_);
    tx->setInt(block_id_, offset_, val_, false);
    tx->unpin(block_id_);
  }

  static int SetIntRecord::writeToLog(log::LogManager* lm, int txnum, 
      file::BlockId& block_id, int offset, int val) {
    int tpos = sizeof(uint32_t);
    int fpos = tpos + sizeof(uint32_t);
    int bpos = fpos + sizeof(uint32_t);
    int opos = bpos + sizeof(uint32_t);
    int vpos = opos + sizeof(uint32_t);
    int reclen = vpos + sizeof(uint32_t);
    auto bytes = std::make_shared<std::vector<char>>(reclen, 0);
    auto p = std::make_unique<file::Page>(bytes);
    p->setInt(0, SETINT);
    p->setInt(tpos, txnum_);
    p->setInt(fpos, block_id_.fileName());
    p->setInt(bpos, block_id_.number());
    p->setInt(opos, offset_);
    p->setInt(vpos, val_);
    return lm->append(*bytes);
  }

  SetStringRecord::SetStringRecord(file::Page* p) {
    int tpos = sizeof(uint32_t);
    txnum_ = p->getInt(tpos);
    int fpos = tpos + sizeof(uint32_t);
    std::string filename = p->getString(fpos);
    int bpos = fpos + file::Page::maxLength(filename.size());
    int blknum = p->getInt(bpos);
    blk_ = file::BlockId(filename, blknum);
    int opos = bpos + sizeof(uint32_t);
    offset = p->getInt(opos);
    int vpos = opos + sizeof(uint32_t);
    val = p->getString(vpos);
  }

  std::string SetStringRecord::toString() {
    return "<SETSTRING" + txnum + " "  + blk_.toString() + " " + offset_ + " " + val_ + ">";
  }

  void SetStringRecord::undo(Transaction* tx) {
    tx->pin(blk_);
    tx->setString(blk_, offset_, val_, false); // don't log the undo
    tx->unpin(blk_);
  }

  static int SetStringRecord::writeToLog(log::LogManager* lm, int txnum, file::BlockId& blk, int offset, std::string val) {
    int tpos = sizeof(uint32_t);
    int fpos = tpos + sizeof(uint32_t);
    int bpos = fpos + file::Page::maxLength(blk.fileName().size());
    int opos = bpos + sizeof(uint32_t);
    int vpos = opos + sizeof(uint32_t);
    int reclen = vpos + file::Page::maxLength(val.size());
    auto rec = std::make_shared<std::vector<char>>(reclen, 0);
    auto p = std::make_unique<file::Page>(rec);
    p->setInt(0, SETSTRING);
    p->setInt(tpos, txnum);
    p->setString(fpos, blk.fileName());
    p->setInt(opos, offset);
    p->setString(vpos, val);
    return lm->append(*rec);
  }
}
