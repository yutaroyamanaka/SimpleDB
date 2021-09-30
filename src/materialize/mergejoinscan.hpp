/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "materialize/sortscan.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"

namespace materialize {
class MergeJoinScan : public scan::Scan {
 public:
    MergeJoinScan(const std::shared_ptr<scan::Scan>& s1, const std::shared_ptr<SortScan>& s2, const std::string& fldname1, const std::string& fldname2);
    void close() override;
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    scan::Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
 private:
    std::shared_ptr<scan::Scan> s1_; 
    std::shared_ptr<SortScan> s2_;
    std::string fldname1_, fldname2_;
    scan::Constant joinval_;
};
}  // namespace materialize
