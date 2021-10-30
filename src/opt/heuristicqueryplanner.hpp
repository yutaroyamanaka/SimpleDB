/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <set>
#include <string>
#include "meta/metadatamanager.hpp"
#include "opt/tableplanner.hpp"
#include "plan/planner.hpp"
#include "plan/projectplan.hpp"
#include "plan/queryplanner.hpp"
#include "parse/querydata.hpp"
#include "tx/transaction.hpp"

namespace opt {
class HeuristicQueryPlanner : public plan::QueryPlanner {
 public:
    HeuristicQueryPlanner(meta::MetaDataManager* mdm);
    std::shared_ptr<plan::Plan> createPlan(const parse::QueryData& data, tx::Transaction* transaction) override;
    void setPlanner(plan::Planner* p);
 private:
    std::set<std::shared_ptr<TablePlanner>> tableplanners_;
    meta::MetaDataManager* mdm_;

    std::shared_ptr<plan::Plan> getLowestSelectPlan();
    std::shared_ptr<plan::Plan> getLowestJoinPlan(const std::shared_ptr<plan::Plan>& current);
    std::shared_ptr<plan::Plan> getLowestProductPlan(const std::shared_ptr<plan::Plan>& current);
};
}  // namespace opt
