#include "buffer.hpp"
#include "buffermanager.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include <memory>
#include <chrono>
#include <thread>

namespace buffer {
  BufferManager::BufferManager(file::FileManager* file_manager, log::LogManager* log_manager, int numbuffs) {

    num_available_ = numbuffs;
    for(int i = 0; i < numbuffs; i++) {
      auto new_buffer = std::make_unique<Buffer>(file_manager, log_manager);
      buffer_pool_.emplace_back(std::move(new_buffer));
    }
  }

  int BufferManager::available() {
    std::unique_lock<std::mutex> lock(mutex_);
    return num_available_;
  }

  void BufferManager::flushAll(int txnum) {
    std::unique_lock<std::mutex> lock(mutex_);
    for(auto&& buff: buffer_pool_) {
      if(buff->modifyingTx() == txnum) {
        buff->flush();
      }
    }
  }

  void BufferManager::unpin(Buffer* buff) {
    std::unique_lock<std::mutex> lock(mutex_);
    buff->unpin();
    if(!buff->isPinned()) {
      num_available_++;
      condition_var_.notify_all();
    }
  }

  Buffer* BufferManager::pin(file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long time_stamp = value.count();
    
    try {
      Buffer* buff = tryToPin(block_id);
      while(!buff && !waitingTooLong(time_stamp)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(MAX_TIME_));
        buff = tryToPin(block_id);
      }
      if(!buff) {
        throw std::runtime_error("buffer abort exception");
      }
      return buff;
    } catch (std::exception& e) {
      throw std::runtime_error("buffer abort exception");
    }
  }

  bool BufferManager::waitingTooLong(long start_time) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long time_stamp = value.count();
    return time_stamp - start_time > MAX_TIME_;
  }

  Buffer* BufferManager::tryToPin(file::BlockId& block_id) {
    Buffer* buff = findExistingBuffer(block_id);
    if(!buff) {
      buff = chooseUpinnedBuffer();
      if(!buff) {
        return nullptr;
      }
      buff->assignToBlock(block_id);
    }
    if(!buff->isPinned()) {
      num_available_--;
    }
    buff->pin();
    return buff;
  }

  Buffer* BufferManager::findExistingBuffer(file::BlockId& block_id) {
    for(auto&& buff: buffer_pool_) {
      file::BlockId b = buff->block();
      if(!b.isNull() && b.equals(block_id)) {
        return buff.get();
      }
    }
    return nullptr;
  }

  Buffer* BufferManager::chooseUpinnedBuffer() {
    for(auto&& buff: buffer_pool_) {
      if(!buff->isPinned()) {
        return buff.get();
      }
    }
    return nullptr;
  }
}
