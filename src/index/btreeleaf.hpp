/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "file/blockid.hpp"
#include "index/btpage.hpp"
#include "index/direntry.hpp"
#include "record/layout.hpp"
#include "record/rid.hpp"
#include "scan/constant.hpp"
#include "tx/transaction.hpp"

namespace index {
class BTreeLeaf {
 public:
    BTreeLeaf();
    BTreeLeaf(tx::Transaction* transaction, file::BlockId& blk, const record::Layout& layout, const scan::Constant& searchkey);
    void close();
    bool next();
    record::RID getDataRid();
    void remove(const record::RID& datarid);
    DirEntry insert(const record::RID& datarid);
    bool isNull();
 private:
    tx::Transaction* transaction_;
    record::Layout layout_;
    scan::Constant searchkey_;
    BTPage contents_;
    int currentslot_;
    std::string filename_;

    bool tryOverflow();
};
}  // namespace index
