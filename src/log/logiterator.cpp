/* Copyright 2021 Yutaro Yamanaka */
#include "log/logmanager.hpp"
#include "log/logiterator.hpp"

namespace log {
  LogIterator::LogIterator(file::FileManager* file_manager, file::BlockId& block_id) : file_manager_(file_manager), block_id_(block_id) {
    auto buf = std::make_shared<std::vector<char>>(file_manager_->blockSize(), 0);
    page_ = std::make_unique<file::Page>(buf);
    moveToBlock(block_id_);
  }

  bool LogIterator::hasNext() {
    return current_pos_ < file_manager_->blockSize() || block_id_.number() > 0;
  }

  std::vector<char> LogIterator::next() {
    if (current_pos_ == file_manager_->blockSize()) {
      block_id_ = file::BlockId(block_id_.fileName(), block_id_.number() - 1);
      moveToBlock(block_id_);
    }

    std::vector<char> rec = page_->getBytes(current_pos_);
    current_pos_ += sizeof(uint32_t) + rec.size();
    return rec;
  }

  void LogIterator::moveToBlock(file::BlockId& block_id) {
    file_manager_->read(block_id, *page_);
    boundary = page_->getInt(0);
    current_pos_ = boundary;
  }
}  // namespace log
