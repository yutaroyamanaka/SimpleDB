/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "plan/plan.hpp"
#include "scan/selectscan.hpp"
#include "scan/predicate.hpp"

namespace plan {
class SelectPlan : public Plan {
 public:
   SelectPlan(const std::shared_ptr<Plan>& p, const scan::Predicate& pred);
   std::shared_ptr<scan::Scan> open() override;
   int blocksAccessed() override;
   int recordsOutput() override;
   int distinctValues(const std::string& fldname) override;
   record::Schema schema() override;
 private:
   std::shared_ptr<Plan> p_;
   scan::Predicate pred_;
};
}  // namespace plan
