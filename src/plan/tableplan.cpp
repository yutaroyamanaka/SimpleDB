/* Copyright 2021 Yutaro Yamanaka */
#include "plan/tableplan.hpp"

namespace plan {
  TablePlan::TablePlan(tx::Transaction* transaction, const std::string& tblname, meta::MetaDataManager* mdm) : transaction_(transaction), tblname_(tblname) {
    layout_ = mdm->getLayout(tblname_, transaction_);
    si_ = mdm->getStatInfo(tblname_, layout_, transaction_);
  }

  std::shared_ptr<scan::Scan> TablePlan::open() {
    auto ts = std::make_shared<record::TableScan>(transaction_, tblname_, layout_);
    return std::static_pointer_cast<scan::Scan>(ts);
  }

  int TablePlan::blocksAccessed() {
    return si_.blocksAccessed();
  }

  int TablePlan::recordsOutput() {
    return si_.recordOutput();
  }

  int TablePlan::distinctValues(const std::string& fldname) {
    return si_.distinctValues(fldname);
  }

  record::Schema TablePlan::schema() {
    return layout_.schema();
  }
}  // namespace plan
