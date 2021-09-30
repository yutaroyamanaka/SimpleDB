/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "scan/constant.hpp"
#include "scan/scan.hpp"

namespace materialize {
class AggregationFn {
 public:
    virtual ~AggregationFn() = default;
    virtual void processFirst(scan::Scan* s) = 0;
    virtual void processNext(scan::Scan* s) = 0;
    virtual std::string fieldName() = 0;
    virtual scan::Constant value() = 0;
};
}  // namespace materialize
