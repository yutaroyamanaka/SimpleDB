/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "scan/constant.hpp"
#include "record/tablescan.hpp"
#include "meta/indexmanager.hpp"
#include "plan/plan.hpp"

namespace index {
class IndexSelectPlan : public plan::Plan {
 public:
    IndexSelectPlan(const std::shared_ptr<Plan>& p, const meta::IndexInfo& ii, const scan::Constant& val);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    std::shared_ptr<Plan> p_;
    meta::IndexInfo ii_;
    scan::Constant val_;
};
}  // namespace index
