/* Copyright 2021 Yutaro Yamanaka */
#include "logging/logmanager.hpp"
#include "logging/logiterator.hpp"

namespace logging {
  LogManager::LogManager(file::FileManager* file_manager, const std::string& logfile) : file_manager_(file_manager), logfile_(logfile) {
    auto buf = std::make_shared<std::vector<char>>(file_manager_->blockSize(), 0);
    logpage_ = std::make_unique<file::Page>(buf);

    int logsize = file_manager_->length(logfile);
    if (logsize == 0) {
      current_block_ = appendNewBlock();
    } else {
      current_block_ = file::BlockId(logfile_, logsize-1);
      file_manager->read(current_block_, *logpage_);
    }
  }

  void LogManager::flush(int lsn) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (lsn >= last_saved_lsn_) {
      flush();
    }
  }

  LogIterator LogManager::iterator() {
    flush();
    LogIterator log_iterator(file_manager_, current_block_);
    return log_iterator;
  }

  int LogManager::append(std::vector<char>& logrec) {
    std::unique_lock<std::mutex> lock(mutex_);
    int boundary = logpage_->getInt(0);
    int recsize = logrec.size();
    int bytesneeded = recsize + sizeof(uint32_t);
    if (boundary - bytesneeded < static_cast<int>(sizeof(uint32_t))) {
      flush();
      current_block_ = appendNewBlock();  // the page is dirty here
      boundary = logpage_->getInt(0);
    }
    int recpos = boundary - bytesneeded;
    logpage_->setBytes(recpos, logrec);
    logpage_->setInt(0, static_cast<uint32_t>(recpos));

    latest_lsn_ += 1;
    return latest_lsn_;
  }

  file::BlockId LogManager::appendNewBlock() {
    file::BlockId block_id = file_manager_->append(logfile_);
    logpage_->setInt(0, file_manager_->blockSize());
    file_manager_->write(block_id, *logpage_);
    return block_id;
  }

  void LogManager::flush() {
    file_manager_->write(current_block_, *logpage_);
    last_saved_lsn_ = latest_lsn_;
  }
}  // namespace logging
