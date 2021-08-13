/* Copyright 2021 Yutaro Yamanaka */
#include "viewmanager.hpp"

namespace meta {
  ViewManager::ViewManager(bool isNew, TableManager* table_manager, tx::Transaction* transaction) : table_manager_(table_manager) {
    if (isNew) {
      record::Schema sch;
      sch.addStringField("viewname", table_manager_->MAX_NAME);
      sch.addStringField("viewdef", MAX_VIEWDEF);
      table_manager_->createTable("viewcat", sch, transaction);
    }
  }

  void ViewManager::createView(const std::string& vname, const std::string& vdef, tx::Transaction* transaction) {
    record::Layout layout = table_manager_->getLayout("viewcat", transaction);
    record::TableScan ts(transaction, "viewcat", layout);
    ts.insert();
    ts.setString("viewname", vname);
    ts.setString("viewdef", vdef);
    ts.close();
  }

  std::string ViewManager::getViewDef(const std::string& vname, tx::Transaction* transaction) const {
    std::string result;
    record::Layout layout = table_manager_->getLayout("viewcat", transaction);
    record::TableScan ts(transaction, "viewcat", layout);
    while (ts.next()) {
      if (ts.getString("viewname") == vname) {
        result = ts.getString("viewdef");
        break;
      }
    }
    ts.close();
    return result;
  }
}  // namespace meta
