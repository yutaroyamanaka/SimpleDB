/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "tablemanager.hpp"
#include "transaction.hpp"
#include "tablescan.hpp"

namespace meta {
class ViewManager {
 public:
    ViewManager(bool isNew, TableManager* table_manager, tx::Transaction* transaction);
    void createView(const std::string& vname, const std::string& vdef, tx::Transaction* transaction);
    std::string getViewDef(const std::string& vname, tx::Transaction* transaction) const;
 private:
    const int MAX_VIEWDEF = 100;
    TableManager* table_manager_;
};
}  // namespace meta
