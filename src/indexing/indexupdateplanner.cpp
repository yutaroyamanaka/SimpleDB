/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/indexupdateplanner.hpp"

namespace indexing {
  IndexUpdatePlanner::IndexUpdatePlanner(meta::MetaDataManager* mdm) : mdm_(mdm) {
  }

  int IndexUpdatePlanner::executeDelete(parse::DeleteData* data, tx::Transaction* transaction) {
    std::string tblname = data->tableName();
    auto p = std::static_pointer_cast<plan::Plan>(std::make_shared<plan::TablePlan>(transaction, tblname, mdm_));
    p = std::static_pointer_cast<plan::Plan>(std::make_shared<plan::SelectPlan>(p, data->pred()));
    std::map<std::string, meta::IndexInfo> indexes = mdm_->getIndexInfo(tblname, transaction);
    auto s = std::static_pointer_cast<scan::UpdateScan>(p->open());
    int count = 0;

    while (s->next()) {
      // first, delete the record's RID from every index
      record::RID rid = s->getRid();
      for (const auto& [fldname, index] : indexes) {
        scan::Constant val = s->getVal(fldname);
        auto idx = indexes[fldname].open();
        idx->remove(val, rid);
        idx->close();
      }
      // then delete the record
      s->remove();
      count++;
    }
    s->close();
    return count;
  }

  int IndexUpdatePlanner::executeModify(parse::ModifyData* data, tx::Transaction* transaction) {
    std::string tblname = data->tableName();
    std::string fldname = data->targetField();
    auto p = std::static_pointer_cast<plan::Plan>(std::make_shared<plan::TablePlan>(transaction, tblname, mdm_));
    p = std::static_pointer_cast<plan::Plan>(std::make_shared<plan::SelectPlan>(p, data->pred()));
    meta::IndexInfo ii = mdm_->getIndexInfo(tblname, transaction).at(fldname);
    auto idx = (ii.isNull()) ? nullptr : ii.open();
    auto s = std::static_pointer_cast<scan::UpdateScan>(p->open());
    int count = 0;
    while (s->next()) {
      scan::Constant newval = data->newValue().evaluate(s.get());
      scan::Constant oldval = s->getVal(fldname);
      s->setVal(data->targetField(), newval);
      // then update the appropriate index, if it exists
      if (idx) {
        record::RID rid = s->getRid();
        idx->remove(oldval, rid);
        idx->insert(newval, rid);
      }
      count++;
    }
    if (idx) idx->close();
    s->close();
    return count;
  }

  int IndexUpdatePlanner::executeInsert(parse::InsertData* data, tx::Transaction* transaction) {
    std::string tblname = data->tableName();
    auto p = std::static_pointer_cast<plan::Plan>(std::make_shared<plan::TablePlan>(transaction, tblname, mdm_));
    // first, insert the record
    auto s = std::static_pointer_cast<scan::UpdateScan>(p->open());
    s->insert();
    record::RID rid = s->getRid();
    // then modify each field, inserting index records
    std::map<std::string, meta::IndexInfo> indexes = mdm_->getIndexInfo(tblname, transaction);
    auto vals = data->vals();
    int i = 0;
    for (const auto fldname : data->fields()) {
      scan::Constant val = vals[i];
      s->setVal(fldname, val);

      meta::IndexInfo ii = indexes[fldname];
      if (!ii.isNull()) {
        auto idx = ii.open();
        idx->insert(val, rid);
        idx->close();
      }
      i++;
    }
    s->close();
    return 1;
  }

  int IndexUpdatePlanner::executeCreateTable(parse::CreateTableData* data, tx::Transaction* transaction) {
    mdm_->createTable(data->tableName(), data->newSchema(), transaction);
    return 0;
  }

  int IndexUpdatePlanner::executeCreateView(parse::CreateViewData* data, tx::Transaction* transaction) {
    mdm_->createView(data->viewName(), data->viewDef(), transaction);
    return 0;
  }

  int IndexUpdatePlanner::executeCreateIndex(parse::CreateIndexData* data, tx::Transaction* transaction) {
    mdm_->createIndex(data->indexName(), data->tableName(), data->fieldName(), transaction);
    return 0;
  }
}  // namespace indexing
