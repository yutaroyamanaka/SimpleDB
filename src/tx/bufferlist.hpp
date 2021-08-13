/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <map>
#include "buffer/buffermanager.hpp"
#include "file/blockid.hpp"

namespace tx {
class BufferList {
 public:
    BufferList(buffer::BufferManager* bm);
    buffer::Buffer* getBuffer(const file::BlockId& block_id);
    void pin(const file::BlockId& block_id);
    void unpin(const file::BlockId& block_id);
    void unpinAll();
 private:
    buffer::BufferManager* bm_;
    std::vector<file::BlockId> pins_;
    std::map<file::BlockId, buffer::Buffer*> buffers_;
};
}  // namespace tx
