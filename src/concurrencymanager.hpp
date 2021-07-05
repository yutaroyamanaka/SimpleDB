#pragma once
#include "locktable.hpp"
#include "blockid.hpp"
#include <string>
#include <map>
#include <memory>

namespace tx {
  class ConcurrencyManager {
    public:
      ConcurrencyManager();
      void sLock(file::BlockId& block_id);
      void xLock(file::BlockId& block_id);
      void release();
    private:
      std::map<file::BlockId, std::string> locks_;
      static LockTable lock_table_;
      bool hasXLock(file::BlockId& block_id);
  };
}
