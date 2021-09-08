/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/indexjoinplan.hpp"

namespace indexing {
  IndexJoinPlan::IndexJoinPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2, const meta::IndexInfo& ii, const std::string& joinfield)
    : p1_(p1), p2_(p2), ii_(ii), joinfield_(joinfield) {
    sch_.addAll(p1->schema());
    sch_.addAll(p2->schema());
  }

  std::shared_ptr<scan::Scan> IndexJoinPlan::open() {
    // throws an exception if p is not a table plan.
    auto s = p1_->open();
    auto ts = std::static_pointer_cast<record::TableScan>(p2_->open());
    auto idx = ii_.open();
    return std::static_pointer_cast<scan::Scan>(std::make_shared<IndexJoinScan>(s, idx, joinfield_, ts));
  }

  int IndexJoinPlan::blocksAccessed() {
    return p1_->blocksAccessed() + p1_->recordsOutput() * ii_.blocksAccessed() + recordsOutput();
  }

  int IndexJoinPlan::recordsOutput() {
    return p1_->recordsOutput() * ii_.recordOutput();
  }

  int IndexJoinPlan::distinctValues(const std::string& fldname) {
    if (p1_->schema().hasField(fldname)) {
      return p1_->distinctValues(fldname);
    } else {
      return p2_->distinctValues(fldname);
    }
  }

  record::Schema IndexJoinPlan::schema() {
    return sch_;
  }
}  // namespace indexing
