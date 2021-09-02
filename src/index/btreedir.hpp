/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "file/blockid.hpp"
#include "index/btpage.hpp"
#include "index/direntry.hpp"
#include "record/layout.hpp"
#include "scan/constant.hpp"
#include "tx/transaction.hpp"

namespace index {
class BTreeDir {
 public:
    BTreeDir(tx::Transaction* transaction, file::BlockId& blk, const record::Layout& layout);
    void close();
    int search(const scan::Constant& searchkey);
    void makeNewRoot(const DirEntry& e);
    DirEntry insert(const DirEntry& e);
 private:
    tx::Transaction* transaction_;
    record::Layout layout_;
    BTPage contents_;
    std::string filename_;

    DirEntry insertEntry(const DirEntry& e);
    file::BlockId findChildBlock(const scan::Constant& searchkey);
};
}  // namespace index
