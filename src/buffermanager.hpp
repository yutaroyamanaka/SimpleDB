#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "blockid.hpp"
#include "page.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "logiterator.hpp"
#include "buffer.hpp"

namespace buffer {
class BufferManager {
  public:
    BufferManager(file::FileManager* file_manager, log::LogManager* long_manager, int num_buffs);
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
}
