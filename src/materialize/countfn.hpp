/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "materialize/aggregationfn.hpp"
#include "parse/word.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"

namespace materialize {
class CountFn : public AggregationFn {
 public:
    CountFn();
    void processFirst(scan::Scan* s) override;
    void processNext(scan::Scan* s) override;
    std::string fieldName() override;
    scan::Constant value() override;
 private:
    scan::Constant val_;
    int cnt_;
};
}  // namespace materialize
