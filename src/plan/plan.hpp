/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include "scan/scan.hpp"
#include "record/schema.hpp"

namespace plan {
class Plan {
 public:
    virtual ~Plan() = default;
    virtual std::shared_ptr<scan::Scan> open() = 0;
    virtual int blocksAccessed() = 0;
    virtual int recordsOutput() = 0;
    virtual int distinctValues(const std::string& fldname) = 0;
    virtual record::Schema schema() = 0;
};
}  // namespace plan
