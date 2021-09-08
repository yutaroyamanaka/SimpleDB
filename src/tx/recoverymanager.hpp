/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <algorithm>
#include <string>
#include <condition_variable>
#include <vector>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "logging/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "buffer/buffer.hpp"
#include "tx/logrecord.hpp"

namespace tx {
class Transaction;

class RecoveryManager {
 public:
    RecoveryManager(Transaction* trx, int txnum, logging::LogManager* lm, buffer::BufferManager* bm);
    void commit();
    void rollback();
    void recover();
    int setInt(buffer::Buffer* buff, int offset, int newval);
    int setString(buffer::Buffer* buff, int offset, const std::string& newval);
 private:
    logging::LogManager* lm_;
    buffer::BufferManager* bm_;
    Transaction* tx_;
    int txnum_;

    void doRollback();
    void doRecover();
};
}  // namespace tx
