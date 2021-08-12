/* Copyright 2021 Yutaro Yamanaka */
#include "concurrencymanager.hpp"

namespace tx {
  LockTable ConcurrencyManager::lock_table_;

  ConcurrencyManager::ConcurrencyManager() {

  }

  void ConcurrencyManager::sLock(const file::BlockId& block_id) {
    if(auto itr = locks_.find(block_id); itr == locks_.end()) {
      lock_table_.sLock(block_id);
      locks_[block_id] = "S";
    }
  }

  void ConcurrencyManager::xLock(const file::BlockId& block_id) {
    if(!hasXLock(block_id)) {
      sLock(block_id);
      lock_table_.xLock(block_id);
      locks_[block_id] = "X";
    }
  }

  void ConcurrencyManager::release() {
    for(auto& item: locks_) {
      lock_table_.unlock(item.first);
    }
    locks_.clear();
  }

  bool ConcurrencyManager::hasXLock(const file::BlockId& block_id) {
    auto itr = locks_.find(block_id);
    if(itr == locks_.end()) return false;
    std::string lock_type = locks_[block_id];
    return lock_type == "X";
  }
}
