/* Copyright 2021 Yutaro Yamanaka */
#include "plan/productplan.hpp"

namespace plan {
  ProductPlan::ProductPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2) : p1_(p1), p2_(p2) {
    schema_.addAll(p1_->schema());
    schema_.addAll(p2_->schema());
  }

  std::shared_ptr<scan::Scan> ProductPlan::open() {
    auto s1 = p1_->open();
    auto s2 = p2_->open();
    return std::static_pointer_cast<scan::Scan>(std::make_shared<scan::ProductScan>(s1, s2));
  }

  int ProductPlan::blocksAccessed() {
    return p1_->blocksAccessed() + (p1_->recordsOutput() * p2_->blocksAccessed());
  }

  int ProductPlan::recordsOutput() {
    return p1_->recordsOutput() * p2_->recordsOutput();
  }

  int ProductPlan::distinctValues(const std::string& fldname) {
    if (p1_->schema().hasField(fldname)) {
      return p1_->distinctValues(fldname);
    } else {
      return p2_->distinctValues(fldname);
    }
  }

  record::Schema ProductPlan::schema() {
    return schema_;
  }
}  // namespace plan
