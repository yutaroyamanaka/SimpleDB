/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "parse/parser.hpp"
#include "parse/querydata.hpp"
#include "plan/queryplanner.hpp"
#include "plan/updateplanner.hpp"
#include "tx/transaction.hpp"

namespace plan {
class Planner {
 public:
    Planner(const std::shared_ptr<QueryPlanner>& qplanner, const std::shared_ptr<UpdatePlanner>& uplanner);
    std::shared_ptr<Plan> createQueryPlan(const std::string& cmd, tx::Transaction* transaction);  
    int executeUpdate(const std::string& cmd, tx::Transaction* transaction);
 private:
    std::shared_ptr<QueryPlanner> qplanner_;
    std::shared_ptr<UpdatePlanner> uplanner_;
};
}  // namespace plan
