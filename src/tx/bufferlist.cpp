/* Copyright 2021 Yutaro Yamanaka */
#include "tx/bufferlist.hpp"

namespace tx {
  BufferList::BufferList(buffer::BufferManager* bm): bm_(bm) {
  }

  buffer::Buffer* BufferList::getBuffer(const file::BlockId& block_id) {
    return buffers_[block_id];
  }

  void BufferList::pin(const file::BlockId& block_id) {
    buffer::Buffer* buff = bm_->pin(block_id);
    buffers_[block_id] = buff;
    pins_.emplace_back(block_id);
  }

  void BufferList::unpin(const file::BlockId& block_id) {
    buffer::Buffer* buff = buffers_[block_id];
    bm_->unpin(buff);

    for (auto itr = pins_.begin(); itr != pins_.end();) {
      if (*itr == block_id) {
        itr = pins_.erase(itr);
        break;
      } else {
        itr++;
      }
    }

    for (auto itr = pins_.begin(); itr != pins_.end(); itr++) {
      if (*itr == block_id) return;
    }
    buffers_.erase(block_id);
  }

  void BufferList::unpinAll() {
    for (auto& blk : pins_) {
      buffer::Buffer* buff = buffers_[blk];
      bm_->unpin(buff);
    }

    buffers_.clear();
    pins_.clear();
  }
}  // namespace tx
