#pragma once
#include <string>
#include <condition_variable>
#include "blockid.hpp"
#include "page.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"
#include "buffer.hpp"

namespace tx {
class RecoveryManager {
  public:
    BufferManager(Transaction* tx, int txnum, log::LogManager* lm, buffer::BufferManager* bm);
    void commit();
    void rollback();
    void recover();
    int setInt(buffer::Buffer* buff, int offset, int newval);
    int setString(buffer::Buffer* buff, int offset, const std::string& newval);
  private:
    log::LogManager* lm_;
    buffer::BufferManager* bm_;
    Transaction* tx_;
    int txnum_;

    void doRollback();
    void doRecover();
};
}
