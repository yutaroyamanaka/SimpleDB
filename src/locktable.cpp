#include "locktable.hpp"
#include <algorithm>

namespace tx {
  LockTable::LockTable() {
  }

  void LockTable::sLock(const file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto start = std::chrono::high_resolution_clock::now();

    while(hasXlock(block_id) && !waitTooLong(start)) {
      condition_var_.wait_for(lock, std::chrono::milliseconds(MAX_TIME));
    }

    if(hasXlock(block_id)) {
      throw std::runtime_error("new lock abort exception");
    }

    int val = getLockVal(block_id);
    locks_[block_id] = val + 1;
  }

  void LockTable::xLock(const file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto start = std::chrono::high_resolution_clock::now();

    while(hasOtherSlocks(block_id) && !waitTooLong(start)) {
      condition_var_.wait_for(lock, std::chrono::milliseconds(MAX_TIME));
    }

    if(hasOtherSlocks(block_id)) {
      throw std::runtime_error("new lock abort exception");
    }

    locks_[block_id] = -1;
  }

  void LockTable::unlock(const file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    int val = getLockVal(block_id);
    if(val > 1) {
      locks_[block_id] = val - 1;
    } else {
      locks_.erase(block_id);
      condition_var_.notify_all();
    }
  }

  bool LockTable::hasXlock(const file::BlockId& block_id) {
    return getLockVal(block_id) < 0;
  }

  bool LockTable::hasOtherSlocks(const file::BlockId& block_id) {
    return getLockVal(block_id) > 1;
  }

  bool LockTable::waitTooLong(std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();
    return elapsed > MAX_TIME;
  }

  int LockTable::getLockVal(const file::BlockId& block_id) {
    auto itr = locks_.find(block_id);
    if(itr == locks_.end()) return 0;
    return locks_[block_id];
  }
}
