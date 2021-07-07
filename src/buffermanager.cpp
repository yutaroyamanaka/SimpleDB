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

  Buffer* BufferManager::pin(const file::BlockId& block_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto start = std::chrono::high_resolution_clock::now();

    Buffer* buff = tryToPin(block_id);
    while(!buff && !waitingTooLong(start)) {
      condition_var_.wait_for(lock, std::chrono::milliseconds(MAX_TIME));
    }
    if(!buff) {
      throw std::runtime_error("buffer abort exeception");
    }
    return buff;
  }

  bool BufferManager::waitingTooLong(std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();
    return elapsed > MAX_TIME;
  }

  Buffer* BufferManager::tryToPin(const file::BlockId& block_id) {
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

  Buffer* BufferManager::findExistingBuffer(const file::BlockId& block_id) {
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
