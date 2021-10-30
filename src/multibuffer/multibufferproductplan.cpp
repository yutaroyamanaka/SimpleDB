/* Copyright 2021 Yutaro Yamanaka */
#include "multibuffer/multibufferproductplan.hpp"

namespace multibuffer {
  MultiBufferProductPlan::MultiBufferProductPlan(const std::shared_ptr<plan::Plan>& lhs, const std::shared_ptr<plan::Plan>& rhs, tx::Transaction* transaction) :
    transaction_(transaction), lhs_(lhs), rhs_(rhs) {
      sch_.addAll(lhs_->schema());
      sch_.addAll(rhs_->schema());
    }

  std::shared_ptr<scan::Scan> MultiBufferProductPlan::open() {
    auto leftscan = lhs_->open();
    auto t = copyRecordsFrom(rhs_);
    return std::static_pointer_cast<scan::Scan>(std::make_shared<MultiBufferProductScan>(leftscan, t->tableName(), t->getLayout(), transaction_));
  }

  int MultiBufferProductPlan::blocksAccessed() {
    // this guesse at the # of chunks
    int avail = transaction_->availableBuffs();
    auto mp = std::make_shared<materialize::MaterializePlan>(transaction_, rhs_);
    int size = mp->blocksAccessed();
    int numchunks = size / avail;
    return rhs_->blocksAccessed() + (lhs_->blocksAccessed() * numchunks);
  }

  int MultiBufferProductPlan::recordsOutput() {
    return lhs_->recordsOutput() * rhs_->recordsOutput();
  }

  int MultiBufferProductPlan::distinctValues(const std::string& fldname) {
    if (lhs_->schema().hasField(fldname)) {
      return lhs_->distinctValues(fldname);
    } else {
      return rhs_->distinctValues(fldname);
    }
  }

  record::Schema MultiBufferProductPlan::schema() {
    return sch_;
  }

  std::shared_ptr<materialize::TempTable> MultiBufferProductPlan::copyRecordsFrom(const std::shared_ptr<plan::Plan>& p) {
    auto src = p->open();
    record::Schema sch = p->schema();
    auto tt = std::make_shared<materialize::TempTable>(transaction_, sch);
    auto dest = tt->open();
    while (src->next()) {
      dest->insert();
      for (const auto& fldname : sch.fields()) {
        dest->setVal(fldname, src->getVal(fldname));
      }
    }
    src->close();
    dest->close();
    return tt;
  }
}  // namespace multibuffer
