/* Copyright 2021 Yutaro Yamanaka */
#include "buffer/buffer.hpp"
#include <memory>
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"

namespace buffer {
  Buffer::Buffer(file::FileManager* file_manager, log::LogManager* log_manager): file_manager_(file_manager), log_manager_(log_manager) {
    contents_ = std::make_unique<file::Page>(file_manager_->blockSize());
  }

  void Buffer::setModified(int txnum, int lsn) {
    txnum_ = txnum;
    if (lsn >= 0) {
      lsn_ = lsn;
    }
  }

  bool Buffer::isPinned() {
    return pins_ > 0;
  }

  int Buffer::modifyingTx() {
    return txnum_;
  }

  void Buffer::assignToBlock(const file::BlockId& block_id) {
    flush();
    block_id_ = block_id;
    file_manager_->read(block_id_, *contents_);
    pins_ = 0;
  }

  void Buffer::flush() {
    if (txnum_ >= 0) {
      log_manager_->flush(lsn_);
      file_manager_->write(block_id_, *contents_);
      txnum_ = -1;
    }
  }

  void Buffer::pin() {
    pins_++;
  }

  void Buffer::unpin() {
    pins_--;
  }
}  // namespace buffer
