/* Copyright 2021 Yutaro Yamanaka */
#include "transaction.hpp"
#include <iostream>

namespace tx {
  int Transaction::nextTxNum_ = 0;
  std::mutex Transaction::mutex_;

  Transaction::Transaction(file::FileManager* fm, log::LogManager* lm, buffer::BufferManager* bm) : fm_(fm), lm_(lm), bm_(bm) {
    txnum_ = nextTxNumber();
    rm_ = std::make_unique<RecoveryManager>(this, txnum_, lm_, bm_);
    cm_ = std::make_unique<ConcurrencyManager>();
    my_buffers_ = std::make_unique<BufferList>(bm_);
  }

  void Transaction::commit() {
    rm_->commit();
    cm_->release();
    my_buffers_->unpinAll();
  }

  void Transaction::rollback() {
    rm_->rollback();
    cm_->release();
    my_buffers_->unpinAll();
  }

  void Transaction::recover() {
    bm_->flushAll(txnum_);
    rm_->recover();
    cm_->release();
    my_buffers_->unpinAll();
  }

  void Transaction::pin(const file::BlockId& block_id) {
    my_buffers_->pin(block_id);
  }

  void Transaction::unpin(const file::BlockId& block_id) {
    my_buffers_->unpin(block_id);
  }

  int Transaction::getInt(const file::BlockId& block_id, int offset) {
    cm_->sLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    return buff->contents()->getInt(offset);
  }

  std::string Transaction::getString(const file::BlockId& block_id, int offset) {
    cm_->sLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    return buff->contents()->getString(offset);
  }

  void Transaction::setInt(const file::BlockId& block_id, int offset,
      int val, bool okToLog) {
    cm_->xLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    int lsn = -1;
    if (okToLog) {
      lsn = rm_->setInt(buff, offset, val);
    }

    auto p = buff->contents();
    p->setInt(offset, val);
    buff->setModified(txnum_, lsn);
  }

  void Transaction::setString(const file::BlockId& block_id, int offset, const std::string& val, bool okToLog) {
    cm_->xLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    int lsn = -1;
    if (okToLog) {
      lsn = rm_->setString(buff, offset, val);
    }
    auto p = buff->contents();
    p->setString(offset, val);
    buff->setModified(txnum_, lsn);
  }

  int Transaction::size(const std::string& filename) {
    file::BlockId dummyblk(filename, END_OF_FILE);
    cm_->sLock(dummyblk);
    return fm_->length(filename);
  }

  file::BlockId Transaction::append(const std::string& filename) {
    file::BlockId dummyblk(filename, END_OF_FILE);
    cm_->xLock(dummyblk);
    return fm_->append(filename);
  }

  int Transaction::blockSize() {
    return fm_->blockSize();
  }

  int Transaction::availableBuffs() {
    return bm_->available();
  }

  void Transaction::forceCMClear() {
    cm_->release();
    my_buffers_->unpinAll();
  }

  int Transaction::nextTxNumber() {
    std::unique_lock<std::mutex> lock(mutex_);
    nextTxNum_++;
    return nextTxNum_;
  }
}  // namespace tx
