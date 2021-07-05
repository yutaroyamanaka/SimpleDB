#pragma once
#include <memory>
#include <mutex>
#include <map>
#include <condition_variable>
#include "blockid.hpp"

namespace tx {
class LockTable {
  public:
    LockTable();
    void sLock(file::BlockId& block_id);
    void xLock(file::BlockId& block_id);
    void unlock(file::BlockId& block_id);
  private:
    static const long MAX_TIME_ = 10000; // 10 sec
    std::mutex mutex_;
    std::map<file::BlockId, int> locks_; 
    std::condition_variable condition_var_;

    bool hasXlock(file::BlockId& block_id);
    bool hasOtherSlocks(file::BlockId& block_id);
    bool waitTooLong(long start_time);
    int getLockVal(file::BlockId& block_id;)
};
}
