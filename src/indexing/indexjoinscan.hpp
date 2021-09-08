/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "indexing/index.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"
#include "record/tablescan.hpp"

namespace indexing {
class IndexJoinScan : public scan::Scan {
 public:
    IndexJoinScan(const std::shared_ptr<scan::Scan>& lhs, const std::shared_ptr<Index>& idx, const std::string& joinfld, const std::shared_ptr<record::TableScan>& rhs);
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    scan::Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
    void close() override;
 private:
    std::shared_ptr<scan::Scan> lhs_;
    std::shared_ptr<Index> idx_;
    std::string joinfld_;
    std::shared_ptr<record::TableScan> rhs_;

    void resetIndex();
};
}  // namespace indexing
