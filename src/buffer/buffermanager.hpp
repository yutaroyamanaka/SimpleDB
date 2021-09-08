/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <utility>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "logging/logmanager.hpp"
#include "logging/logiterator.hpp"
#include "buffer/buffer.hpp"

namespace buffer {
class BufferManager {
 public:
    BufferManager(file::FileManager* file_manager, logging::LogManager* long_manager, int num_buffs);
    int available();
    void flushAll(int txnum);
    void unpin(Buffer* buff);
    Buffer* pin(const file::BlockId& block_id);
 private:
    const int MAX_TIME = 10000;
    int num_available_;
    std::vector<std::unique_ptr<buffer::Buffer>> buffer_pool_;
    std::mutex mutex_;
    std::condition_variable condition_var_;

    bool waitingTooLong(std::chrono::time_point<std::chrono::high_resolution_clock> start_time);
    Buffer* tryToPin(const file::BlockId& block_id);
    Buffer* findExistingBuffer(const file::BlockId& block_id);
    Buffer* chooseUpinnedBuffer();
};
}  // namespace buffer
