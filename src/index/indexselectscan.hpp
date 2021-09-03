/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "index/index.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"
#include "record/tablescan.hpp"

namespace index {
class IndexSelectScan : public scan::Scan {
 public:
    IndexSelectScan(const std::shared_ptr<record::TableScan>& ts, const std::shared_ptr<index::Index>& idx, const scan::Constant& val);
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    scan::Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
    void close() override;
 private:
    std::shared_ptr<record::TableScan> ts_;
    std::shared_ptr<index::Index> idx_;
    scan::Constant val_;
};
}  // namespace index
