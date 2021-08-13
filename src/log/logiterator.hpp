/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <memory>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"

namespace log {
class LogIterator {
 public:
    LogIterator(file::FileManager* file_manager, file::BlockId& block_id);
    bool hasNext();
    std::vector<char> next();
 private:
    file::FileManager* file_manager_;
    std::unique_ptr<file::Page> page_;
    file::BlockId block_id_;
    int current_pos_;
    int boundary;
    void moveToBlock(file::BlockId& block_id);
};
}  // namespace log
