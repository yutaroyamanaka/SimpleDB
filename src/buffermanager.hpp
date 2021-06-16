#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
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
    Buffer* pin(file::BlockId& block_id);
  private:
    const long MAX_TIME_ = 5000;
    int num_available_;
    std::vector<std::unique_ptr<buffer::Buffer>> buffer_pool_;
    std::mutex mutex_;
    std::condition_variable condition_var_;

    bool waitingTooLong(long start_time);
    Buffer* tryToPin(file::BlockId& block_id);
    Buffer* findExistingBuffer(file::BlockId& block_id);
    Buffer* chooseUpinnedBuffer();
};
}
