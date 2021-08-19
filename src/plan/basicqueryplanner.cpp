/* Copyright 2021 Yutaro Yamanaka */
#include "plan/basicqueryplanner.hpp"

namespace plan {
  BasicQueryPlanner::BasicQueryPlanner(meta::MetaDataManager* mdm) : mdm_(mdm) {
  }

  std::shared_ptr<Plan> BasicQueryPlanner::createPlan(const parse::QueryData& data, tx::Transaction* transaction) {
    std::vector<std::shared_ptr<Plan>> plans;
    for (const auto& tblname : data.tables()) {
      std::string viewdef = mdm_->getViewDef(tblname, transaction);
      if (!viewdef.empty()) {  // recursively plan the view
        parse::Parser parser(viewdef);
        parse::QueryData viewdata = parser.query();
        plans.emplace_back(createPlan(viewdata, transaction));
      } else {
        auto tp = std::make_shared<TablePlan>(transaction, tblname, mdm_);
        plans.emplace_back(tp);
      }
    }

    auto p = plans[0];
    plans.erase(plans.begin());
    for (const auto& nextplan : plans) {
      p = std::static_pointer_cast<Plan>(std::make_shared<ProductPlan>(p, nextplan));
    }

    p = std::static_pointer_cast<Plan>(std::make_shared<SelectPlan>(p, data.pred()));
    return std::static_pointer_cast<Plan>(std::make_shared<ProjectPlan>(p, data.fields()));
  }
}  // namespace plan
