/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include "constant.hpp"
#include "updatescan.hpp"
#include "scan.hpp"
#include "predicate.hpp"

namespace scan {
class SelectScan: public UpdateScan {
 public:
    SelectScan(std::shared_ptr<Scan>& s, const Predicate& pred);
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
    void close() override;
    void setInt(const std::string& fldname, int val) override;
    void setString(const std::string& fldname, const std::string& val) override;
    void setVal(const std::string& fldname, const Constant& val) override;
    void insert() override;
    void remove() override;
    void moveToRid(const record::RID& rid) override;
    record::RID getRid() override;

 private:
    std::shared_ptr<Scan> s_;
    Predicate pred_;
};
}  // namespace scan
