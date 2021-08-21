/* Copyright 2021 Yutaro Yamanaka */
#include "parse/createtabledata.hpp"

namespace parse {
  CreateTableData::CreateTableData(const std::string& tblname, const record::Schema& sch) : tblname_(tblname), sch_(sch) {
  }

  std::string CreateTableData::tableName() const {
    return tblname_;
  }

  record::Schema CreateTableData::newSchema() const {
    return sch_;
  }

  int CreateTableData::getID() {
    return id_;
  }
}  // namespace parse
