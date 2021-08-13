/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <mutex>
#include <memory>
#include <iostream>
#include "concurrencymanager.hpp"
#include "recoverymanager.hpp"
#include "buffermanager.hpp"
#include "filemanager.hpp"
#include "bufferlist.hpp"
#include "blockid.hpp"
#include "page.hpp"

namespace tx {
class RecoveryManager;

class Transaction {
 public:
    Transaction(file::FileManager* fm, log::LogManager* lm, buffer::BufferManager* bm);
    void commit();
    void rollback();
    void recover();
    void pin(const file::BlockId& block_id);
    void unpin(const file::BlockId& block_id);
    int getInt(const file::BlockId& block_id, int offset);
    std::string getString(const file::BlockId& block_id, int offset);
    void setInt(const file::BlockId& block_id, int offset, int val,
        bool okToLog);
    void setString(const file::BlockId& block_id, int offset,
        const std::string& val, bool okToLog);
    int size(const std::string& filename);
    file::BlockId append(const std::string& filename);
    int blockSize();
    int availableBuffs();
    int getTransactionNum() { return txnum_; }
    void forceCMClear();

 private:
    static int nextTxNum_;
    static const int END_OF_FILE = -1;
    std::unique_ptr<RecoveryManager> rm_;
    std::unique_ptr<ConcurrencyManager> cm_;
    buffer::BufferManager* bm_;
    log::LogManager* lm_;
    file::FileManager* fm_;
    int txnum_;
    std::unique_ptr<BufferList> my_buffers_;
    static std::mutex mutex_;
    static int nextTxNumber();
};
}  // namespace tx
