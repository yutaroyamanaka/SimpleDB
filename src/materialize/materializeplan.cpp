/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/materializeplan.hpp"

namespace materialize {
  MaterializePlan::MaterializePlan(tx::Transaction* transaction, std::shared_ptr<plan::Plan>& srcplan) : transaction_(transaction), srcplan_(srcplan) {
  }

  std::shared_ptr<scan::Scan> MaterializePlan::open() {
    record::Schema sch = srcplan_->schema();
    auto temp = std::make_shared<TempTable>(transaction_, sch);
    auto src = srcplan_->open();
    auto dest = temp->open();
    while (src->next()) {
      dest->insert();
      for (const auto& fldname : sch.fields()) {
        auto c = src->getVal(fldname);
        dest->setVal(fldname, src->getVal(fldname));
      }
    }
    src->close();
    dest->beforeFirst();
    return std::static_pointer_cast<scan::Scan>(dest);
  }

  int MaterializePlan::blocksAccessed() {
    // create a dummy layout object to calculate slot size
    record::Layout y(srcplan_->schema());
    double rgb = static_cast<double>(transaction_->blockSize() / y.slotSize());
    return static_cast<int>(std::ceil(srcplan_->recordsOutput() / rgb));
  }

  int MaterializePlan::recordsOutput() {
    return srcplan_->recordsOutput();
  }

  int MaterializePlan::distinctValues(const std::string& fldname) {
    return srcplan_->distinctValues(fldname);
  }

  record::Schema MaterializePlan::schema() {
    return srcplan_->schema();
  }
}  // namespace materialize
