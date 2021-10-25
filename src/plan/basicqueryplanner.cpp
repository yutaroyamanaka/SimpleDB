/* Copyright 2021 Yutaro Yamanaka */
#include "plan/basicqueryplanner.hpp"

namespace plan {
  BasicQueryPlanner::BasicQueryPlanner(meta::MetaDataManager* mdm) : mdm_(mdm) {
  }

  std::shared_ptr<Plan> BasicQueryPlanner::createPlan(const parse::QueryData& data, tx::Transaction* transaction) {
    std::vector<std::shared_ptr<Plan>> plans;
    for (const auto& tblname : data.tables()) {
      std::string viewdef = mdm_->getViewDef(tblname, transaction);
      record::Schema schema = mdm_->getLayout(tblname, transaction).schema();

      if (!viewdef.empty()) {  // recursively plan the view
        parse::Parser parser(viewdef);
        parse::QueryData viewdata = parser.query();
        plans.emplace_back(createPlan(viewdata, transaction));
      } else {
        auto tp = std::make_shared<TablePlan>(transaction, tblname, mdm_);
        plans.emplace_back(std::static_pointer_cast<Plan>(tp));

        if (data.groupFields().size() == 0) continue;  // not add aggregation functions

        for (const auto& fldname : schema.fields()) {
          aggfns_.emplace_back(std::static_pointer_cast<materialize::AggregationFn>(std::make_shared<materialize::MaxFn>(fldname)));
          aggfns_.emplace_back(std::static_pointer_cast<materialize::AggregationFn>(std::make_shared<materialize::MinFn>(fldname)));
          aggfns_.emplace_back(std::static_pointer_cast<materialize::AggregationFn>(std::make_shared<materialize::SumFn>(fldname)));
        }
      }
    }

    for (const auto& subdata : data.queryTables()) {
      auto srcplan = createPlan(subdata, transaction);
      auto mp = std::make_shared<materialize::MaterializePlan>(transaction, srcplan);
      plans.emplace_back(std::static_pointer_cast<Plan>(mp));
    }

    auto p = plans[0];
    plans.erase(plans.begin());
    for (const auto& nextplan : plans) {
      p = std::static_pointer_cast<Plan>(std::make_shared<ProductPlan>(p, nextplan));
    }

    p = std::static_pointer_cast<Plan>(std::make_shared<SelectPlan>(p, data.pred()));
    if (data.groupFields().size() > 0) {
      aggfns_.emplace_back(std::static_pointer_cast<materialize::AggregationFn>(std::make_shared<materialize::CountFn>()));
      p = std::static_pointer_cast<Plan>(std::make_shared<materialize::GroupByPlan>(transaction, p, data.groupFields(), aggfns_));
    }
    p = std::static_pointer_cast<Plan>(std::make_shared<ProjectPlan>(p, data.fields()));

    if (data.sortFields().size() > 0) {
      /*
      if (data.groupFields().size() > 0) {
        p = std::static_pointer_cast<Plan>(std::make_shared<materialize::MaterializePlan>(transaction, p));
      } */
      // p = std::static_pointer_cast<Plan>(std::make_shared<materialize::SortPlan>(p, data.sortFields(), transaction));
      p = std::static_pointer_cast<Plan>(std::make_shared<multibuffer::MultiBufferSortPlan>(p, data.sortFields(), transaction));
    }
    return p;
  }
}  // namespace plan
