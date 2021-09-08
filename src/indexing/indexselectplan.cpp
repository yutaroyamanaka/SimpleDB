/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/indexselectplan.hpp"

namespace indexing {
  IndexSelectPlan::IndexSelectPlan(const std::shared_ptr<Plan>& p, const meta::IndexInfo& ii, const scan::Constant& val) : p_(p), ii_(ii), val_(val) {
  }

  std::shared_ptr<scan::Scan> IndexSelectPlan::open() {
    // throws an exception if p is not a table plan.
    auto ts = std::static_pointer_cast<record::TableScan>(p_->open());
    auto idx = ii_.open();
    return std::static_pointer_cast<scan::Scan>(std::make_shared<IndexSelectScan>(ts, idx, val_));
  }

  int IndexSelectPlan::blocksAccessed() {
    return ii_.blocksAccessed() + recordsOutput();
  }

  int IndexSelectPlan::recordsOutput() {
    return ii_.recordOutput();
  }

  int IndexSelectPlan::distinctValues(const std::string& fldname) {
    return ii_.distinctValues(fldname);
  }

  record::Schema IndexSelectPlan::schema() {
    return p_->schema();
  }
}  // namespace indexing
