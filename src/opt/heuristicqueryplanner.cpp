/* Copyright 2021 Yutaro Yamanaka */
#include "opt/heuristicqueryplanner.hpp"

namespace opt {
  HeuristicQueryPlanner::HeuristicQueryPlanner(meta::MetaDataManager* mdm) : mdm_(mdm) {
  }

  std::shared_ptr<plan::Plan> HeuristicQueryPlanner::createPlan(const parse::QueryData& data, tx::Transaction* transaction) {
    // Step 1, Create a TablePlanner object for each mentioned table
    for (const auto& tblname : data.tables()) {
      auto tp = std::make_shared<TablePlanner>(tblname, data.pred(), transaction, mdm_);
      tableplanners_.insert(tp);
    }

    // Step 2, Choose the lowest-size plan to begin the join order
    auto currentPlan = getLowestSelectPlan();

    // Step 3, Repeatedly add a plan to the join order
    while (tableplanners_.size() != 0) {
      auto p = getLowestJoinPlan(currentPlan);
      if (p) {
        currentPlan = p;
      } else {
        // no applicable join
        currentPlan = getLowestProductPlan(currentPlan);
      }
    }

    // Step 4, Project on the field names and return
    return std::static_pointer_cast<plan::Plan>(std::make_shared<plan::ProjectPlan>(currentPlan, data.fields()));
  }

    void HeuristicQueryPlanner::setPlanner(plan::Planner* p) {
      // for use in planning views, which
      // for simplicity this code doesn't do
    }

    std::shared_ptr<plan::Plan> HeuristicQueryPlanner::getLowestSelectPlan() {
      std::shared_ptr<TablePlanner> besttp = nullptr;
      std::shared_ptr<plan::Plan> bestplan = nullptr;
      for (const auto& tp : tableplanners_) {
        auto plan = tp->makeSelectPlan();
        if (!bestplan || plan->recordsOutput() < bestplan->recordsOutput()) {
          besttp = tp;
          bestplan = plan;
        }
      }
      tableplanners_.erase(besttp);
      return bestplan;
    }

    std::shared_ptr<plan::Plan> HeuristicQueryPlanner::getLowestJoinPlan(const std::shared_ptr<plan::Plan>& current) {
      std::shared_ptr<TablePlanner> besttp = nullptr;
      std::shared_ptr<plan::Plan> bestplan = nullptr;
      for (const auto& tp : tableplanners_) {
        auto plan = tp->makeJoinPlan(current);
        if (!bestplan || plan->recordsOutput() < bestplan->recordsOutput()) {
          besttp = tp;
          bestplan = plan;
        }
      }

      if (bestplan) tableplanners_.erase(besttp);
      return bestplan;
    }

    std::shared_ptr<plan::Plan> HeuristicQueryPlanner::getLowestProductPlan(const std::shared_ptr<plan::Plan>& current) {
      std::shared_ptr<TablePlanner> besttp = nullptr;
      std::shared_ptr<plan::Plan> bestplan = nullptr;
      for (const auto& tp : tableplanners_) {
        auto plan = tp->makeProductPlan(current);
        if (!bestplan || plan->recordsOutput() < bestplan->recordsOutput()) {
          besttp = tp;
          bestplan = plan;
        }
      }
      tableplanners_.erase(besttp);
      return bestplan;
    }
}  // namespace opt
