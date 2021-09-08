/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "indexing/indexjoinscan.hpp"
#include "scan/constant.hpp"
#include "record/tablescan.hpp"
#include "record/schema.hpp"
#include "meta/indexmanager.hpp"
#include "plan/plan.hpp"

namespace indexing {
class IndexJoinPlan : public plan::Plan {
 public:
    IndexJoinPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2, const meta::IndexInfo& ii, const std::string& joinfield);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    std::shared_ptr<Plan> p1_;
    std::shared_ptr<Plan> p2_;
    meta::IndexInfo ii_;
    std::string joinfield_;
    record::Schema sch_;
};
}  // namespace indexing
