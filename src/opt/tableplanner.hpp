/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <map>
#include <memory>
#include <string>
#include "indexing/indexselectplan.hpp"
#include "indexing/indexjoinplan.hpp"
#include "meta/indexmanager.hpp"
#include "meta/metadatamanager.hpp"
#include "multibuffer/multibufferproductplan.hpp"
#include "plan/planner.hpp"
#include "plan/selectplan.hpp"
#include "plan/tableplan.hpp"
#include "scan/predicate.hpp"
#include "tx/transaction.hpp"

namespace opt {
class TablePlanner {
 public:
    TablePlanner(const std::string& tblname, const scan::Predicate& mypred, tx::Transaction* transaction, meta::MetaDataManager* mdm);
    std::shared_ptr<plan::Plan> makeSelectPlan();
    std::shared_ptr<plan::Plan> makeJoinPlan(const std::shared_ptr<plan::Plan>& current);
    std::shared_ptr<plan::Plan> makeProductPlan(const std::shared_ptr<plan::Plan>& current);
 private:
    std::shared_ptr<plan::TablePlan> myplan_;
    scan::Predicate mypred_;
    tx::Transaction* transaction_;
    record::Schema mysch_;
    std::map<std::string, meta::IndexInfo> indexes_;

    std::shared_ptr<plan::Plan> makeIndexSelect();
    std::shared_ptr<plan::Plan> makeIndexJoin(const std::shared_ptr<plan::Plan>& current, const record::Schema& currsch);
    std::shared_ptr<plan::Plan> makeProductJoin(const std::shared_ptr<plan::Plan>& current, const record::Schema& currsch);
    std::shared_ptr<plan::Plan> addSelectPred(const std::shared_ptr<plan::Plan>& p);
    std::shared_ptr<plan::Plan> addJoinPred(const std::shared_ptr<plan::Plan>& p, const record::Schema& currsch);
};
}  // namespace opt
