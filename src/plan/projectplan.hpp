/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "scan/projectscan.hpp"

namespace plan {
class ProjectPlan : public Plan {
 public:
   ProjectPlan(const std::shared_ptr<Plan>& p, const std::vector<std::string>& fieldList);
   std::shared_ptr<scan::Scan> open() override;
   int blocksAccessed() override;
   int recordsOutput() override;
   int distinctValues(const std::string& fldname) override;
   record::Schema schema() override;
 private:
   std::shared_ptr<Plan> p_;
   record::Schema schema_;
};
}  // namespace plan
