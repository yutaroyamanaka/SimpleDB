/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/aggregationfn.hpp"
#include "materialize/groupvalue.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"

namespace materialize {
class GroupByScan : public scan::Scan {
 public:
    GroupByScan(const std::shared_ptr<scan::Scan>& s, const std::vector<std::string>& groupFields, const std::vector<std::shared_ptr<AggregationFn>>& aggfns);
    void beforeFirst() override;
    bool next() override;
    void close() override;
    scan::Constant getVal(const std::string& fldname) override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
 private:
    std::shared_ptr<scan::Scan> s_;
    std::vector<std::string> groupFields_;
    std::vector<std::shared_ptr<AggregationFn>> aggfns_;
    std::unique_ptr<GroupValue> groupVal_;
    bool moreGroups_;
};
}  // namespace materialize
