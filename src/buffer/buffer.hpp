/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "log/logiterator.hpp"

namespace buffer {
class Buffer {
 public:
    Buffer(file::FileManager* file_manager, log::LogManager* log_manager);
    file::Page* contents() { return contents_.get(); }
    file::BlockId block() { return block_id_; }
    void setModified(int txnum, int lsn);
    bool isPinned();
    int modifyingTx();
    void assignToBlock(const file::BlockId& block_id);
    void flush();
    void pin();
    void unpin();
 private:
    file::FileManager* file_manager_;
    log::LogManager* log_manager_;
    std::unique_ptr<file::Page> contents_;
    file::BlockId block_id_;
    int pins_ = 0;
    int txnum_ = -1;
    int lsn_ = -1;
};
}  // namespace buffer
