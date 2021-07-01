#pragma once
#include <string>
#include <condition_variable>
#include <memory>
#include "blockid.hpp"
#include "page.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"
#include "buffer.hpp"

namespace tx {
class LogRecord {
  public:
    virtual ~LogRecord();
    virtual int op();
    virtual int txNumber();
    virtual std::string toString();
    virtual void undo(int txnum);
    static std::unique_ptr<LogRecord> createLogRecord(std::shared_ptr<std::vector<char>>& bytes); 
    static const int CHECKPOINT = 0, START = 1, COMMIT = 2, ROLLBACK = 3, SETINT = 4, SETSTRING = 5;
};

class CheckpointRecord : LogRecord {
  public:
    CheckpointRecord();
    int op() override { return CHECKPOINT; }
    int txNumber() override { return -1; }
    std::string toString() override { return "<CHECKPOINT>"; }
    void undo(Transaction* tx) override {}
    static int writeToLog(log::LogManager* lm);
};

class StartRecord : LogRecord {
  public:
    StartRecord(file::Page* page);
    int op() override { return START; }
    int txNumber() override { return txnum_; }
    std::string toString() override { return "<START," + std::to_string(txnum_) + ">"; }
    void undo(Transaction* tx) override {}
    static int writeToLog(log::LogManager* lm, int txnum);
  private:
    int txnum_;
};

class CommitRecord : LogRecord {
  public:
    CommitRecord(file::Page* page);
    int op() override { return COMMIT; }
    int txNumber() override { return txnum_; }
    std::string toString() override { return "<COMMIT, " + std::to_string(txnum_) +  ">"; }
    void undo(Transaction* tx) override {}
    static int writeToLog(log::LogManager* lm, int txnum);
  private:
    int txnum_;
};

class RollbackRecord : LogRecord {
  public:
    RollbackRecord(file::Page* p);
    int op() override { return ROLLBACK; }
    int txNumber() override { return txnum_; }
    std::string toString() override { return "<COMMIT, >" + std::to_string(txnum_) + ">"; }
    void undo(Transaction* tx) override {}
    static int writeToLog(log::LogManager* lm, int txnum);
  private:
    int txnum_;
};

class SetIntRecord : LogRecord {
  public:
    SetIntRecord(file::Page* p);
    int op() override { return SETINT; }
    int txNumber() override { return txnum_; }
    std::string toString() override;
    void undo(Transaction* tx) override;
    static int writeToLog(log::LogManager* lm, int txnum, file::BlockId& block_id, int offset, int val);
  private:
    int txnum_;
    int val_;
    int offset_;
    file::BlockId block_id_;
}

class SetStringRecord : LogRecord {
  public:
    SetStringRecord(file::Page* p);
    int op() override { return SETSTRING; }
    int txNumber() override { return txnum_; }
    std::string toString() override;
    void undo(Transaction* tx) override;
    static int writeToLog(log::LogManager* lm, int txnum, file::BlockId& blk, int offset, std::string val);
  private:
    int txnum_;
    int offset_;
    std::string val_;
    file::BlockId blk_;
};
}
