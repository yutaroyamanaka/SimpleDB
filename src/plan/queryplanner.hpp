/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include "parse/querydata.hpp"
#include "plan/plan.hpp"
#include "tx/transaction.hpp"

namespace plan {
class QueryPlanner {
 public:
    ~QueryPlanner() = default;
    virtual std::shared_ptr<Plan> createPlan(const parse::QueryData& data, tx::Transaction* transaction) = 0;  
};
}  // namespace plan
