/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "file/blockid.hpp"
#include "record/layout.hpp"
#include "record/recordpage.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"
#include "tx/transaction.hpp"

namespace multibuffer {
class ChunkScan : public scan::Scan {
 public:
    ChunkScan(tx::Transaction* transaction, const std::string& filename, const record::Layout& layout, int startbnum, int endbnum);
    void close() override;
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    scan::Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
 private:
    void moveToBlock(int blknum);
    std::vector<std::unique_ptr<record::RecordPage>> buffs_;
    tx::Transaction* transaction_;
    std::string filename_;
    record::Layout layout_;
    int startbnum_, endbnum_, currentbnum_;
    record::RecordPage* rp_;
    int currentslot_;
};
}  // namespace multibuffer
