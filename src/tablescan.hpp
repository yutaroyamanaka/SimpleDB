/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include "transaction.hpp"
#include "updatescan.hpp"
#include "scan.hpp"
#include "layout.hpp"
#include "recordpage.hpp"
#include "rid.hpp"
#include "constant.hpp"

namespace record {
class TableScan : public scan::UpdateScan {
 public:
    ~TableScan();
    TableScan(tx::Transaction* transaction, const std::string& tblname, const Layout& layout);
    void close() override;
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    scan::Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
    void setInt(const std::string& fldname, int val) override;
    void setString(const std::string& fldname, const std::string& val) override;
    void setVal(const std::string& fldname, const scan::Constant& val) override;
    void insert() override;
    void remove() override;
    void moveToRid(const RID& rid) override;
    RID getRid() override;

 private:
    void moveToBlock(int blknum);
    void moveToNewBlock();
    bool atLastBlock();

    tx::Transaction* tx_;
    Layout layout_;
    std::unique_ptr<RecordPage> record_page_;
    std::string filename_;
    int current_slot_;
};
}  // namespace record
