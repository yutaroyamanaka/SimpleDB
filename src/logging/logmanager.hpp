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
#include "logging/logiterator.hpp"

namespace logging {
class LogManager {
 public:
    LogManager(file::FileManager* file_manager, const std::string& logfile);
    void flush(int lsn);
    LogIterator iterator();
    int append(std::vector<char>& logrec);
    int getLastSavedLSN() { return last_saved_lsn_; }
 private:
    file::FileManager* file_manager_;
    std::string logfile_;
    std::unique_ptr<file::Page> logpage_;
    file::BlockId current_block_;
    int latest_lsn_ = 0;
    int last_saved_lsn_ = 0;
    std::mutex mutex_;

    file::BlockId appendNewBlock();
    void flush();
};
}  // namespace logging
