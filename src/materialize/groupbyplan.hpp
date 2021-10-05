/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/aggregationfn.hpp"
#include "materialize/groupbyscan.hpp"
#include "materialize/sortplan.hpp"
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "scan/scan.hpp"
#include "tx/transaction.hpp"

namespace materialize {
class GroupByPlan : public plan::Plan {
 public:
    GroupByPlan(tx::Transaction* transaction, const std::shared_ptr<plan::Plan>& p, const std::vector<std::string>& groupFields, const std::vector<std::shared_ptr<AggregationFn>>& aggfns);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    std::shared_ptr<plan::Plan> p_;
    std::vector<std::string> groupFields_;
    std::vector<std::shared_ptr<AggregationFn>> aggfns_;
    record::Schema sch_;
};
}  // namespace materialize
