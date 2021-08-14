/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <condition_variable>
#include <memory>
#include <vector>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "buffer/buffer.hpp"
#include "tx/transaction.hpp"

namespace tx {
class Transaction;

class LogRecord {
 public:
    virtual ~LogRecord() = default;
    virtual int op() = 0;
    virtual int txNumber() = 0;
    virtual std::string toString() = 0;
    virtual void undo(Transaction* trx) = 0;
    static std::unique_ptr<LogRecord> createLogRecord(const std::vector<char>& bytes);
    static const int CHECKPOINT = 0, START = 1, COMMIT = 2, ROLLBACK = 3, SETINT = 4, SETSTRING = 5;
};

class CheckpointRecord : public LogRecord {
 public:
    CheckpointRecord();
    int op() override { return CHECKPOINT; }
    int txNumber() override { return -1; }
    std::string toString() override { return "<CHECKPOINT>"; }
    void undo(Transaction* trx) override { return; }
    static int writeToLog(log::LogManager* lm);
};

class StartRecord : public LogRecord {
 public:
    StartRecord(file::Page* page);
    int op() override { return START; }
    int txNumber() override { return txnum_; }
    std::string toString() override { return "<START, " + std::to_string(txnum_) + ">"; }
    void undo(Transaction* trx) override { return; }
    static int writeToLog(log::LogManager* lm, int txnum);
 private:
    int txnum_;
};

class CommitRecord : public LogRecord {
 public:
    CommitRecord(file::Page* page);
    int op() override { return COMMIT; }
    int txNumber() override { return txnum_; }
    std::string toString() override { return "<COMMIT, " + std::to_string(txnum_) +  ">"; }
    void undo(Transaction* trx) override { return; }
    static int writeToLog(log::LogManager* lm, int txnum);
 private:
    int txnum_;
};

class RollbackRecord : public LogRecord {
 public:
    RollbackRecord(file::Page* p);
    int op() override { return ROLLBACK; }
    int txNumber() override { return txnum_; }
    std::string toString() override { return "<COMMIT, " + std::to_string(txnum_) + ">"; }
    void undo(Transaction* trx) override { return; }
    static int writeToLog(log::LogManager* lm, int txnum);
 private:
    int txnum_;
};

class SetIntRecord : public LogRecord {
 public:
    SetIntRecord(file::Page* p);
    int op() override { return SETINT; }
    int txNumber() override { return txnum_; }
    std::string toString() override;
    void undo(Transaction* trx) override;
    static int writeToLog(log::LogManager* lm, int txnum, file::BlockId& blk, int offset, int val);
 private:
    int txnum_;
    int val_;
    int offset_;
    file::BlockId block_id_;
};

class SetStringRecord : public LogRecord {
 public:
    SetStringRecord(file::Page* p);
    int op() override { return SETSTRING; }
    int txNumber() override { return txnum_; }
    std::string toString() override;
    void undo(Transaction* trx) override;
    static int writeToLog(log::LogManager* lm, int txnum, file::BlockId& blk, int offset, std::string val);
 private:
    int txnum_;
    int offset_;
    std::string val_;
    file::BlockId block_id_;
};
}  // namespace tx
