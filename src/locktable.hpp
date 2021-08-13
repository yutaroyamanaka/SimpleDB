/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <mutex>
#include <map>
#include <chrono>
#include <condition_variable>
#include <thread>
#include "blockid.hpp"

namespace tx {
class LockTable {
 public:
    LockTable();
    void sLock(const file::BlockId& block_id);
    void xLock(const file::BlockId& block_id);
    void unlock(const file::BlockId& block_id);
 private:
    const int MAX_TIME = 10000;  // 10 sec
    std::mutex mutex_;
    std::map<file::BlockId, int> locks_;
    std::condition_variable condition_var_;

    bool hasXlock(const file::BlockId& block_id);
    bool hasOtherSlocks(const file::BlockId& block_id);
    bool waitTooLong(std::chrono::time_point<std::chrono::high_resolution_clock> start_time);
    int getLockVal(const file::BlockId& block_id);
};
}  // namespace tx
