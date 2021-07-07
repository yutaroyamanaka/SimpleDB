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
    std::cout << "transaction " << std::to_string(txnum_) << " commited" << std::endl;
  }

  void Transaction::rollback() {
    rm_->rollback();
    cm_->release();
    my_buffers_->unpinAll();
    std::cout << "transaction " << std::to_string(txnum_) << " roll back" << std::endl;
  }

  void Transaction::recover() {
    bm_->flushAll(txnum_);
    rm_->recover();
  }

  void Transaction::pin(file::BlockId& block_id) {
    my_buffers_->pin(block_id);
  }

  void Transaction::unpin(file::BlockId& block_id) {
    my_buffers_->unpin(block_id);
  }

  int Transaction::getInt(file::BlockId& block_id, int offset) {
    cm_->sLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    return buff->contents()->getInt(offset);
  }

  std::string Transaction::getString(file::BlockId& block_id, int offset) {
    cm_->sLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    return buff->contents()->getString(offset);
  }

  void Transaction::setInt(file::BlockId& block_id, int offset,
      int val, bool okToLog) {
    cm_->xLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    int lsn = -1;
    if(okToLog) {
      lsn = rm_->setInt(buff, offset, val);
    }

    auto p = buff->contents();
    p->setInt(offset, val);
    buff->setModified(txnum_, lsn);
  }

  void Transaction::setString(file::BlockId& block_id, int offset, std::string val, bool okToLog) {
    cm_->xLock(block_id);
    buffer::Buffer* buff = my_buffers_->getBuffer(block_id);
    int lsn = -1;
    if(okToLog) {
      lsn = rm_->setString(buff, offset, val);
    }
    auto p = buff->contents();
    p->setString(offset, val);
    buff->setModified(txnum_, lsn);
  }

  int Transaction::size(std::string filename) {
    file::BlockId dummyblk(filename, ENF_OF_FILE);
    cm_->sLock(dummyblk);
    return fm_->length(filename);
  }

  file::BlockId Transaction::append(std::string filename) {
    file::BlockId dummyblk(filename, ENF_OF_FILE);
    cm_->xLock(dummyblk);
    return fm_->append(filename);
  }

  int Transaction::blockSize() {
    return fm_->blockSize();
  }

  int Transaction::availableBuffs() {
    return bm_->available();
  }

  int Transaction::nextTxNumber() {
    std::unique_lock<std::mutex> lock(mutex_);
    nextTxNum_++;
    std::cout << "new transaction: " << std::to_string(nextTxNum_) << std::endl;
    return nextTxNum_;
  }
}
