/* Copyright 2021 Yutaro Yamanaka */
#include "plan/planner.hpp"
#include <iostream>

namespace plan {
  Planner::Planner(std::unique_ptr<QueryPlanner> qplanner, std::unique_ptr<UpdatePlanner> uplanner)
    : qplanner_(std::move(qplanner)), uplanner_(std::move(uplanner)) {
  }

  std::shared_ptr<Plan> Planner::createQueryPlan(const std::string& cmd, tx::Transaction* transaction) {
    parse::Parser parser(cmd);
    parse::QueryData data = parser.query();
    return std::static_pointer_cast<Plan>(qplanner_->createPlan(data, transaction));
  }

  int Planner::executeUpdate(const std::string& cmd, tx::Transaction* transaction) {
    parse::Parser parser(cmd);
    auto objptr = parser.updateCmd();
    int id = objptr->getID();

    if (id == parse::ObjectID::INSERT) {
      return uplanner_->executeInsert(static_cast<parse::InsertData*>(objptr.get()), transaction);
    } else if (id == parse::ObjectID::REMOVE) {
      return uplanner_->executeDelete(static_cast<parse::DeleteData*>(objptr.get()), transaction);
    } else if (id == parse::ObjectID::MODIFY) {
      return uplanner_->executeModify(static_cast<parse::ModifyData*>(objptr.get()), transaction);
    } else if (id == parse::ObjectID::CREATE_TABLE) {
      return uplanner_->executeCreateTable(static_cast<parse::CreateTableData*>(objptr.get()), transaction);
    } else if (id == parse::ObjectID::CREATE_VIEW) {
      return uplanner_->executeCreateView(static_cast<parse::CreateViewData*>(objptr.get()), transaction);
    } else if (id == parse::ObjectID::CREATE_INDEX) {
      return uplanner_->executeCreateIndex(static_cast<parse::CreateIndexData*>(objptr.get()),  transaction);
    } else {
      return 0;
    }
  }
}  // namespace plan
