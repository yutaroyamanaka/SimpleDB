/* Copyright 2021 Yutaro Yamanaka */
#include "plan/selectplan.hpp"

namespace plan {
  SelectPlan::SelectPlan(const std::shared_ptr<Plan>& p, const scan::Predicate& pred) : p_(p), pred_(pred) {
  }

  std::shared_ptr<scan::Scan> SelectPlan::open() {
    auto s = p_->open();
    return std::static_pointer_cast<scan::Scan>(std::make_shared<scan::SelectScan>(s, pred_));
  }

  int SelectPlan::blocksAccessed() {
    return p_->blocksAccessed();
  }

  int SelectPlan::recordsOutput() {
    return p_->recordsOutput() / pred_.reductionFactor(p_.get());
  }

  int SelectPlan::distinctValues(const std::string& fldname) {
    if (!pred_.equatesWithConstant(fldname).isNull()) {
      return 1;
    } else {
      std::string fldname2 = pred_.equatesWithField(fldname);
      if (!fldname2.empty()) {
        return std::min(p_->distinctValues(fldname), p_->distinctValues(fldname2));
      }
      return p_->distinctValues(fldname);
    }
  }

  record::Schema SelectPlan::schema() {
    return p_->schema();
  }
}  // namespace plan
