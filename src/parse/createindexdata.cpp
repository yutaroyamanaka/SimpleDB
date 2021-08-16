/* Copyright 2021 Yutaro Yamanaka */
#include "parse/createindexdata.hpp"

namespace parse {
  CreateIndexData::CreateIndexData(const std::string& idxname, const std::string& tblname, const std::string& fldname)
    : idxname_(idxname), tblname_(tblname), fldname_(fldname) {
  }

  std::string CreateIndexData::indexName() const {
    return idxname_;
  }

  std::string CreateIndexData::tableName() const {
    return tblname_;
  }

  std::string CreateIndexData::fieldName() const {
    return fldname_;
  }
}  // namespace parse
