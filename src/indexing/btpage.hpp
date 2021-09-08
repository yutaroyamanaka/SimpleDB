/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "file/blockid.hpp"
#include "record/layout.hpp"
#include "record/rid.hpp"
#include "scan/constant.hpp"
#include "tx/transaction.hpp"

namespace indexing {
class BTPage {
 public:
    BTPage();
    BTPage(tx::Transaction* transaction, const file::BlockId& currentblk, const record::Layout& layout);
    int findSlotBefore(const scan::Constant& searchkey);
    void close();
    bool isFull();
    file::BlockId split(int splitpos, int flagg);
    scan::Constant getDataVal(int slot);
    int getFlag();
    void setFlag(int val);
    file::BlockId appendNew(int flag);
    void format(const file::BlockId& blk, int flag);
    void makeDefaultRecord(const file::BlockId& blk, int pos);
    int getChildNum(int slot);
    void insertDir(int slot, const scan::Constant& val, int blknum);
    record::RID getDataRid(int slot);
    void insertLeaf(int slot, const scan::Constant& val, const record::RID& rid);
    void remove(int slot);
    int getNumRecs();
    bool isNull();

 private:
    tx::Transaction* transaction_;
    file::BlockId currentblk_;
    record::Layout layout_;

    int getInt(int slot, const std::string& fldname);
    std::string getString(int slot, const std::string& fldname);
    scan::Constant getVal(int slot, const std::string& fldname);
    void setInt(int slot, const std::string& fldname, int val);
    void setString(int slot, const std::string& fldname, const std::string& val);
    void setVal(int slot, const std::string& fldname, const scan::Constant& val);
    void setNumRecs(int n);
    void insert(int slot);
    void copyRecord(int from, int to);
    void transferRecs(int slot, BTPage& dest);
    int fldpos(int slot, const std::string& fldname);
    int slotpos(int slot);
};
}  // namespace indexing
