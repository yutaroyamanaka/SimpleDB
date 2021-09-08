/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <climits>
#include <cmath>
#include <string>
#include "file/blockid.hpp"
#include "indexing/btreeleaf.hpp"
#include "indexing/btreedir.hpp"
#include "indexing/btpage.hpp"
#include "indexing/index.hpp"
#include "scan/constant.hpp"
#include "tx/transaction.hpp"
#include "record/layout.hpp"
#include "record/schema.hpp"

namespace indexing {
class BTreeIndex : public Index {
 public:
    BTreeIndex(tx::Transaction* transaction, const std::string& idxname, const record::Layout& leafLayout);
    void beforeFirst(const scan::Constant& searchkey) override;
    bool next() override;
    record::RID getDataRid() override;
    void insert(const scan::Constant& dataval, const record::RID& datarid) override;
    void remove(const scan::Constant& dataval, const record::RID& datarid) override;
    void close() override;
    static int searchCost(int numblocks, int rpb);
 private:
    tx::Transaction* transaction_;
    record::Layout dirLayout_, leafLayout_;
    std::string leaftbl_;
    BTreeLeaf leaf_;
    file::BlockId rootblk_;
};
}  // namespace indexing
