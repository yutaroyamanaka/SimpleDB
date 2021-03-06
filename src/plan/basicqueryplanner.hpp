/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/aggregationfn.hpp"
#include "materialize/countfn.hpp"
#include "materialize/maxfn.hpp"
#include "materialize/minfn.hpp"
#include "materialize/sumfn.hpp"
#include "materialize/groupbyplan.hpp"
#include "materialize/materializeplan.hpp"
#include "materialize/sortplan.hpp"
#include "meta/metadatamanager.hpp"
#include "multibuffer/multibuffersortplan.hpp"
#include "parse/parser.hpp"
#include "parse/querydata.hpp"
#include "plan/selectplan.hpp"
#include "plan/plan.hpp"
#include "plan/productplan.hpp"
#include "plan/projectplan.hpp"
#include "plan/queryplanner.hpp"
#include "plan/tableplan.hpp"
#include "tx/transaction.hpp"

namespace plan {
class BasicQueryPlanner : public QueryPlanner {
 public:
    BasicQueryPlanner(meta::MetaDataManager* mdm);
    std::shared_ptr<Plan> createPlan(const parse::QueryData& data, tx::Transaction* transaction) override;
 private:
    meta::MetaDataManager* mdm_;
    std::vector<std::shared_ptr<materialize::AggregationFn>> aggfns_;
};
}  // namespace plan
