/* Copyright 2021 Yutaro Yamanaka */
#include "plan/projectplan.hpp"

namespace plan {
  ProjectPlan::ProjectPlan(const std::shared_ptr<Plan>& p, const std::vector<std::string>& fieldList) : p_(p) {
    for (const auto& fldname : fieldList) {
      schema_.add(fldname, p_->schema());
    }
  }

  std::shared_ptr<scan::Scan> ProjectPlan::open() {
    auto s = p_->open();
    return std::static_pointer_cast<scan::Scan>(std::make_shared<scan::ProjectScan>(s, schema_.fields()));
  }

  int ProjectPlan::blocksAccessed() {
    return p_->blocksAccessed();
  }

  int ProjectPlan::recordsOutput() {
    return p_->recordsOutput();
  }

  int ProjectPlan::distinctValues(const std::string& fldname) {
    return p_->distinctValues(fldname);
  }

  record::Schema ProjectPlan::schema() {
    return p_->schema();
  }
}  // namespace plan
