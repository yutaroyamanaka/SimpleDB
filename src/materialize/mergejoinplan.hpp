/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include "materialize/mergejoinscan.hpp"
#include "materialize/sortplan.hpp"
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "tx/transaction.hpp"

namespace materialize {
class MergeJoinPlan : public plan::Plan {
 public:
    MergeJoinPlan(tx::Transaction* transaction, const std::shared_ptr<plan::Plan>& p1, const std::shared_ptr<plan::Plan>& p2,
        const std::string& fldname1, const std::string& fldname2);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    std::shared_ptr<plan::Plan> p1_;
    std::shared_ptr<plan::Plan> p2_;
    std::string fldname1_, fldname2_;
    record::Schema sch_;
};
}  // namespace materialize
