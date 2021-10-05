/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/groupbyplan.hpp"

namespace materialize {
  GroupByPlan::GroupByPlan(tx::Transaction* transaction, const std::shared_ptr<plan::Plan>& p,
      const std::vector<std::string>& groupFields, const std::vector<std::shared_ptr<AggregationFn>>& aggfns) :
    groupFields_(groupFields), aggfns_(aggfns) {
      p_ = std::static_pointer_cast<plan::Plan>(std::make_shared<SortPlan>(p, groupFields_, transaction));
      for (const auto& fldname : groupFields_) {
        sch_.add(fldname, p_->schema());
      }
      for (const auto& fn : aggfns_) {
        sch_.addIntField(fn->fieldName());
      }
    }

  std::shared_ptr<scan::Scan> GroupByPlan::open() {
    auto s = p_->open();
    return std::static_pointer_cast<scan::Scan>(std::make_shared<GroupByScan>(s, groupFields_, aggfns_));
  }

  int GroupByPlan::blocksAccessed() {
    return p_->blocksAccessed();
  }

  int GroupByPlan::recordsOutput() {
    int numgroups = 1;
    for (const auto& fldname : groupFields_) {
      numgroups *= p_->distinctValues(fldname);
    }
    return numgroups;
  }

  int GroupByPlan::distinctValues(const std::string& fldname) {
    if (p_->schema().hasField(fldname)) {
      return p_->distinctValues(fldname);
    }
    return recordsOutput();
  }

  record::Schema GroupByPlan::schema() {
    return sch_;
  }
}  // namespace materialize
