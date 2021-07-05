#include "concurrencymanager.hpp"

namespace tx {
  ConcurrencyManager::ConcurrencyManager() {

  }

  void ConcurrencyManager::sLock(file::BlockId& block_id) {
    if(auto itr = locks_.find(block_id); itr == locks_.end()) {
      lock_table_.sLock(block_id);
      locks_[block_id] = "S";
    }
  }

  void ConcurrencyManager::xLock(file::BlockId& block_id) {
    if(!hasXLock(block_id)) {
      sLock(block_id);
      lock_table_.xLock(block_id);
      locks_[block_id] = "X";
    }
  }

  void ConcurrencyManager::release() {
    for(auto itr = lock_table_.begin(); itr != lock_table_.end(); itr++) {
      lock_table_.unlock(block_id);
    }
    locks_.clear();
  }

  bool ConcurrencyManager::hasXLock(file::BlockId& block_id) {
    auto itr = locks_.find(block_id);
    if(itr == locks_.end()) return false;
    std::string lock_type = locks_[block_id];
    return lock_type.compare("X");
  }
}
