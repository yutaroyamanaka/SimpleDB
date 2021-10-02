/* Copyright 2021 Yutaro Yamanaka */
#include "parse/tabledata.hpp"

namespace parse {
  TableData::TableData() {
  }

  std::set<std::string> TableData::getTableNames() const {
    return tableNames_;
  }

  std::vector<QueryData> TableData::getQueryDataList() {
    return queryDataList_;
  }

  void TableData::addTableName(const std::string& tablename) {
    tableNames_.insert(tablename);
  }

  void TableData::addQueryData(QueryData& qd) {
    queryDataList_.emplace_back(qd);
  }
}  // namespace parse
