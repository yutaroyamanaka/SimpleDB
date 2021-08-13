/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include <vector>
#include "scan/constant.hpp"
#include "scan/updatescan.hpp"
#include "scan/scan.hpp"
#include "scan/predicate.hpp"

namespace scan {
class ProductScan: public Scan {
 public:
    ProductScan(std::shared_ptr<Scan>& s1, std::shared_ptr<Scan>& s2);
    void beforeFirst() override;
    bool next() override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    Constant getVal(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
    void close() override;
 private:
    std::shared_ptr<Scan> s1_;
    std::shared_ptr<Scan> s2_;
};
}  // namespace scan
