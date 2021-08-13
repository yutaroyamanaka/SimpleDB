/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <map>
#include <memory>
#include "locktable.hpp"
#include "blockid.hpp"

namespace tx {
class ConcurrencyManager {
 public:
    ConcurrencyManager();
    void sLock(const file::BlockId& block_id);
    void xLock(const file::BlockId& block_id);
    void release();
 private:
    std::map<file::BlockId, std::string> locks_;
    static LockTable lock_table_;
    bool hasXLock(const file::BlockId& block_id);
};
}  // namespace tx
