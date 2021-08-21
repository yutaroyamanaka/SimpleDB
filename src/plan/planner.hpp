/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "parse/objectid.hpp"
#include "parse/parser.hpp"
#include "parse/querydata.hpp"
#include "plan/queryplanner.hpp"
#include "plan/updateplanner.hpp"
#include "tx/transaction.hpp"

namespace plan {
class Planner {
 public:
    Planner(std::unique_ptr<QueryPlanner> qplanner, std::unique_ptr<UpdatePlanner> uplanner);
    std::shared_ptr<Plan> createQueryPlan(const std::string& cmd, tx::Transaction* transaction);
    int executeUpdate(const std::string& cmd, tx::Transaction* transaction);
 private:
    std::unique_ptr<QueryPlanner> qplanner_;
    std::unique_ptr<UpdatePlanner> uplanner_;
};
}  // namespace plan
