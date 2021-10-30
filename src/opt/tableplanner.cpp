/* Copyright 2021 Yutaro Yamanaka */
#include "opt/tableplanner.hpp"

namespace opt {
  TablePlanner::TablePlanner(const std::string& tblname, const scan::Predicate& mypred, tx::Transaction* transaction, meta::MetaDataManager* mdm) :
    mypred_(mypred), transaction_(transaction) {
      myplan_ = std::make_shared<plan::TablePlan>(transaction_, tblname, mdm);
      mysch_ = myplan_->schema();
      indexes_ = mdm->getIndexInfo(tblname, transaction_);
    }

  std::shared_ptr<plan::Plan> TablePlanner::makeSelectPlan() {
    auto p = makeIndexSelect();
    if (!p) p = myplan_;
    return addSelectPred(p);
  }

  std::shared_ptr<plan::Plan> TablePlanner::makeJoinPlan(const std::shared_ptr<plan::Plan>& current) {
    record::Schema currsch = current->schema();
    scan::Predicate joinpred = mypred_.joinSubPred(mysch_, currsch);
    if (joinpred.isNull()) {
      return nullptr;
    }
    auto p = makeIndexJoin(current, currsch);
    if (!p) {
      p = makeProductJoin(current, currsch);
    }
    return p;
  }

  std::shared_ptr<plan::Plan> TablePlanner::makeProductPlan(const std::shared_ptr<plan::Plan>& current) {
    auto p = addSelectPred(myplan_);
    return std::static_pointer_cast<plan::Plan>(std::make_shared<multibuffer::MultiBufferProductPlan>(current, p, transaction_));
  }

  std::shared_ptr<plan::Plan> TablePlanner::makeIndexSelect() {
    for (const auto& [fldname, _] : indexes_) {
      scan::Constant val = mypred_.equatesWithConstant(fldname);
      if (!val.isNull()) {
        meta::IndexInfo ii = indexes_.at(fldname);
        return std::static_pointer_cast<plan::Plan>(std::make_shared<indexing::IndexSelectPlan>(myplan_, ii, val));
      }
    }

    return nullptr;
  }

  std::shared_ptr<plan::Plan> TablePlanner::makeIndexJoin(const std::shared_ptr<plan::Plan>& current, const record::Schema& currsch) {
    for (const auto& [fldname, _] : indexes_) {
      std::string outerfield = mypred_.equatesWithField(fldname);
      if (outerfield.size() != 0 && currsch.hasField(outerfield)) {
        meta::IndexInfo ii = indexes_.at(fldname);
        auto p = std::static_pointer_cast<plan::Plan>(std::make_shared<indexing::IndexJoinPlan>(current, myplan_, ii, outerfield));
        p = addSelectPred(p);
        return addJoinPred(p, currsch);
      }
    }
    return nullptr;
  }

  std::shared_ptr<plan::Plan> TablePlanner::makeProductJoin(const std::shared_ptr<plan::Plan>& current, const record::Schema& currsch) {
    auto p = makeProductPlan(current);
    return addJoinPred(p, currsch);
  }

  std::shared_ptr<plan::Plan> TablePlanner::addSelectPred(const std::shared_ptr<plan::Plan>& p) {
    scan::Predicate selectpred = mypred_.selectSubPred(mysch_);
    if (!selectpred.isNull()) {
      return std::static_pointer_cast<plan::Plan>(std::make_shared<plan::SelectPlan>(p, selectpred));
    } else {
      return p;
    }
  }

  std::shared_ptr<plan::Plan> TablePlanner::addJoinPred(const std::shared_ptr<plan::Plan>& p, const record::Schema& currsch) {
    scan::Predicate joinpred = mypred_.joinSubPred(currsch, mysch_);
    if (!joinpred.isNull()) {
      return std::static_pointer_cast<plan::Plan>(std::make_shared<plan::SelectPlan>(p, joinpred));
    } else {
      return p;
    }
  }

}  // namespace opt
