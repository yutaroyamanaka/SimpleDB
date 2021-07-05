#include "locktable.hpp"

namespace tx {
  LockTable::LockTable() {
  }

  void LockTable::sLock(file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long time_stamp = value.count();

    while(hasXlock(block_id) && !waitTooLong(time_stamp)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_TIME_));
    }

    if(hasXlock(block_id)) {
      throw std::runtime_error("new lock abort exception");
    }

    int val = getLockVal(block_id);
    locks_[block_id] = val + 1;
  }

  void LockTable::xLock(file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long time_stamp = value.count();

    while(hasOtherSlocks(block_id) && !waitTooLong(time_stamp)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_TIME_));
    }

    if(haxOtherSlocks(block_id)) {
      throw std::runtime_error("new lock abort exception");
    }

    locks_[block_id] = -1;
  }

  void LockTable::unlock(file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    int val = getLockVal(block_id);
    if(val > 1) {
      locks_[block_id] = val - 1;
    } else {
      locks_.erase(block_id);
      condition_var_.notify_all();
    }
  }

  bool LockTable::hasXlock(file::BlockId& block_id) {
    return getLockVal(block_id) < 0;
  }

  bool LockTable::hasOtherSlocks(file::BlockId& block_id) {
    return getLockVal(block_id) > 1;
  }

  bool LockTable::waitTooLong(long start_time) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long time_stamp = value.count();
    return time_stamp - start_time > MAX_TIME_;
  }

  int LockTable::getLockVal(file::BlockId& block_id) {
    auto itr = locks_.find(block_id;)
    if(itr == locks_.end()) return 0;
    return locks_[block_id];
  }
}
