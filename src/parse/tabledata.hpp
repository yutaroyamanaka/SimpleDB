/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <set>
#include <string>
#include <vector>
#include "parse/querydata.hpp"

namespace parse {  
class TableData {
 public:
    TableData();
    std::set<std::string> getTableNames() const;
    std::vector<QueryData> getQueryDataList();
    void addTableName(const std::string& tablename);
    void addQueryData(QueryData& qd);
 private:
    std::set<std::string> tableNames_;
    std::vector<QueryData> queryDataList_;
};
}  // namespace parse
