/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "scan/productscan.hpp"

namespace plan {
class ProductPlan : public Plan {
 public:
    ProductPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    std::shared_ptr<Plan> p1_;
    std::shared_ptr<Plan> p2_;
    record::Schema schema_;
};
}  // namespace plan
