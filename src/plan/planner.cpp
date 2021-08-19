/* Copyright 2021 Yutaro Yamanaka */
#include "plan/planner.hpp"

namespace plan {
  Planner::Planner(const std::shared_ptr<QueryPlanner>& qplanner, const std::shared_ptr<UpdatePlanner>& uplanner) : qplanner_(qplanner), uplanner_(uplanner) {
  }

  std::shared_ptr<Plan> Planner::createQueryPlan(const std::string& cmd, tx::Transaction* transaction) {
    parse::Parser parser(cmd);
    parse::QueryData data = parser.query();
    return std::static_pointer_cast<Plan>(qplanner_.get()->createPlan(data, transaction));
  }

  int Planner::executeUpdate(const std::string& cmd, tx::Transaction* transaction) {
    parse::Parser parser(cmd);
    auto obj = std::make_unique<parse::Object>(parser.updateCmd());
    if (dynamic_cast<parse::InsertData*>(obj.get()) != nullptr) { 
      return uplanner_->executeInsert(*(static_cast<parse::InsertData*>(obj.get())),  transaction);
    } else if (dynamic_cast<parse::DeleteData*>(obj.get()) != nullptr) {
      return uplanner_->executeDelete(*(static_cast<parse::DeleteData*>(obj.get())),  transaction);
    } else if (dynamic_cast<parse::ModifyData*>(obj.get()) != nullptr) {
      return uplanner_->executeModify(*(static_cast<parse::ModifyData*>(obj.get())),  transaction);
    } else if (dynamic_cast<parse::CreateTableData*>(obj.get()) != nullptr) {
      return uplanner_->executeCreateTable(*(static_cast<parse::CreateTableData*>(obj.get())),  transaction);
    } else if (dynamic_cast<parse::CreateViewData*>(obj.get()) != nullptr) {
      return uplanner_->executeCreateView(*(static_cast<parse::CreateViewData*>(obj.get())),  transaction);
    } else if (dynamic_cast<parse::CreateIndexData*>(obj.get()) != nullptr) {
      return uplanner_->executeCreateIndex(*(static_cast<parse::CreateIndexData*>(obj.get())),  transaction);
    } else {
      return 0;
    }
  }
}  // namespace plan
