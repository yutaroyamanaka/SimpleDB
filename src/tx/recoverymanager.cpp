/* Copyright 2021 Yutaro Yamanaka */
#include "tx/recoverymanager.hpp"

namespace tx {
  RecoveryManager::RecoveryManager(Transaction* trx, int txnum,
      logging::LogManager* lm, buffer::BufferManager* bm) : tx_(trx), txnum_(txnum), lm_(lm), bm_(bm) {
    StartRecord::writeToLog(lm, txnum_);
  }

  void RecoveryManager::commit() {
    bm_->flushAll(txnum_);
    int lsn = CommitRecord::writeToLog(lm_, txnum_);
    lm_->flush(lsn);
  }

  void RecoveryManager::rollback() {
    doRollback();
    bm_->flushAll(txnum_);
    int lsn = RollbackRecord::writeToLog(lm_, txnum_);
    lm_->flush(lsn);
  }

  void RecoveryManager::recover() {
    doRecover();
    bm_->flushAll(txnum_);
    int lsn = CheckpointRecord::writeToLog(lm_);
    lm_->flush(lsn);
  }

  int RecoveryManager::setInt(buffer::Buffer* buff, int offset, int newval) {
    int oldval = buff->contents()->getInt(offset);
    file::BlockId blk = buff->block();
    return SetIntRecord::writeToLog(lm_, txnum_, blk, offset, oldval);
  }

  int RecoveryManager::setString(buffer::Buffer* buff, int offset, const std::string& newval) {
    std::string oldval = buff->contents()->getString(offset);
    file::BlockId blk = buff->block();
    return SetStringRecord::writeToLog(lm_, txnum_, blk, offset, oldval);
  }

  void RecoveryManager::doRollback() {
    auto iter = lm_->iterator();
    while (iter.hasNext()) {
      auto bytes = iter.next();
      auto rec = LogRecord::createLogRecord(bytes);
      if (rec->txNumber() == txnum_) {
        if (rec->op() == LogRecord::START) return;
        rec->undo(tx_);
      }
    }
  }

  void RecoveryManager::doRecover() {
    std::vector<int> finishd_txs;
    auto iter = lm_->iterator();
    while (iter.hasNext()) {
      auto bytes = iter.next();
      auto rec = LogRecord::createLogRecord(bytes);
      if (rec->op() == LogRecord::CHECKPOINT) {
        return;
      }
      if (rec->op() == LogRecord::COMMIT || rec->op() == LogRecord::ROLLBACK) {
        finishd_txs.emplace_back(rec->txNumber());
      } else if (std::find(finishd_txs.begin(), finishd_txs.end(), rec->txNumber()) == finishd_txs.end())  {
        rec->undo(tx_);
      }
    }
  }
}  // namespace tx
