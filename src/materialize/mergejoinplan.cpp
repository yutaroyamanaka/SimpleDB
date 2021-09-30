/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/mergejoinplan.hpp"

namespace materialize {
  MergeJoinPlan::MergeJoinPlan(tx::Transaction* transaction, const std::shared_ptr<plan::Plan>& p1, const std::shared_ptr<plan::Plan>& p2, const std::string& fldname1, const std::string& fldname2) :
    fldname1_(fldname1), fldname2_(fldname2) {
      std::vector<std::string> sortlist1 = {fldname1_};
      p1_ = std::static_pointer_cast<plan::Plan>(std::make_shared<SortPlan>(p1, sortlist1, transaction));
      std::vector<std::string> sortlist2 = {fldname2_};
      p2_ = std::static_pointer_cast<plan::Plan>(std::make_shared<SortPlan>(p2, sortlist2, transaction));
      sch_.addAll(p1_->schema());
      sch_.addAll(p2_->schema());
    }

  std::shared_ptr<scan::Scan> MergeJoinPlan::open() {
    auto s1 = p1_->open();
    auto s2 = std::static_pointer_cast<SortScan>(p2_->open());
    return std::static_pointer_cast<scan::Scan>(std::make_shared<MergeJoinScan>(s1, s2, fldname1_, fldname2_));
  }

  int MergeJoinPlan::blocksAccessed() {
    return p1_->blocksAccessed() + p2_->blocksAccessed();
  }

  int MergeJoinPlan::recordsOutput() {
    int maxvals = std::max(p1_->distinctValues(fldname1_), p2_->distinctValues(fldname2_));
    return (p1_->recordsOutput() * p2_->recordsOutput()) / maxvals;
  }

  int MergeJoinPlan::distinctValues(const std::string& fldname) {
    if (p1_->schema().hasField(fldname)) return p1_->distinctValues(fldname);
    return p2_->distinctValues(fldname);
  }

  record::Schema MergeJoinPlan::schema() {
    return sch_;
  }
}  // namespace materialize
