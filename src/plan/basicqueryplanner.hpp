/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/materializeplan.hpp"
#include "materialize/sortplan.hpp"
#include "meta/metadatamanager.hpp"
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
};
}  // namespace plan
