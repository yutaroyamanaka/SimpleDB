/* Copyright 2021 Yutaro Yamanaka */
#include "parse/deletedata.hpp"

namespace parse {
  DeleteData::DeleteData(const std::string& tblname, const scan::Predicate& pred) : tblname_(tblname), pred_(pred) {
  }

  std::string DeleteData::tableName() const {
    return tblname_;
  }

  scan::Predicate DeleteData::pred() const {
    return pred_;
  }
}  // namespace parse
