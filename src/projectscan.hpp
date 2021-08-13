/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include <vector>
#include "constant.hpp"
#include "updatescan.hpp"
#include "scan.hpp"
#include "predicate.hpp"

namespace scan {
class ProjectScan: public Scan {
 public:
    ProjectScan(std::shared_ptr<Scan>& s, const std::vector<std::string>& field_list);
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
    void close() override;
 private:
    std::shared_ptr<Scan> s_;
    std::vector<std::string> field_list_;
};
}  // namespace scan
