/* Copyright 2021 Yutaro Yamanaka */
#include "plan/basicupdateplanner.hpp"

namespace plan {
  BasicUpdatePlanner::BasicUpdatePlanner(meta::MetaDataManager* mdm) : mdm_(mdm) {
  }

  int BasicUpdatePlanner::executeDelete(parse::DeleteData* data, tx::Transaction* transaction) {
    auto p = std::static_pointer_cast<Plan>(std::make_shared<TablePlan>(transaction, data->tableName(), mdm_));
    p = std::static_pointer_cast<Plan>(std::make_shared<SelectPlan>(p, data->pred()));
    auto us = std::static_pointer_cast<scan::UpdateScan>(p->open());
    int count = 0;
    while (us->next()) {
      us->remove();
      count++;
    }
    us->close();
    return count;
  }

  int BasicUpdatePlanner::executeModify(parse::ModifyData* data, tx::Transaction* transaction) {
    auto p = std::static_pointer_cast<Plan>(std::make_shared<TablePlan>(transaction, data->tableName(), mdm_));
    p = std::static_pointer_cast<Plan>(std::make_shared<SelectPlan>(p, data->pred()));
    auto us = std::static_pointer_cast<scan::UpdateScan>(p->open());
    int count = 0;
    while (us->next()) {
      scan::Constant val = data->newValue().evaluate(us.get());
      us->setVal(data->targetField(), val);
      count++;
    }
    us->close();
    return count;
  }

  int BasicUpdatePlanner::executeInsert(parse::InsertData* data, tx::Transaction* transaction) {
    auto p = std::static_pointer_cast<Plan>(std::make_shared<TablePlan>(transaction, data->tableName(), mdm_));
    auto us = std::static_pointer_cast<scan::UpdateScan>(p->open());
    us->insert();
    auto vals = data->vals();
    int i = 0;
    for (const auto fldname : data->fields()) {
      scan::Constant val = vals[i];
      us->setVal(fldname, val);
      i++;
    }
    us->close();
    return 1;
  }

  int BasicUpdatePlanner::executeCreateTable(parse::CreateTableData* data, tx::Transaction* transaction) {
    mdm_->createTable(data->tableName(), data->newSchema(), transaction);
    return 0;
  }

  int BasicUpdatePlanner::executeCreateView(parse::CreateViewData* data, tx::Transaction* transaction) {
    mdm_->createView(data->viewName(), data->viewDef(), transaction);
    return 0;
  }

  int BasicUpdatePlanner::executeCreateIndex(parse::CreateIndexData* data, tx::Transaction* transaction) {
    mdm_->createIndex(data->indexName(), data->tableName(), data->fieldName(), transaction);
    return 0;
  }
}  // namespace plan
