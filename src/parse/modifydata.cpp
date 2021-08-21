/* Copyright 2021 Yutaro Yamanaka */
#include "parse/modifydata.hpp"

namespace parse {
  ModifyData::ModifyData(const std::string& tblname, const std::string& fldname, const scan::Expression& newval, const scan::Predicate& pred)
    : tblname_(tblname), fldname_(fldname), newval_(newval), pred_(pred) {
    }

  std::string ModifyData::tableName() const {
    return tblname_;
  }

  std::string ModifyData::targetField() const {
    return fldname_;
  }

  scan::Expression ModifyData::newValue() const {
    return newval_;
  }

  scan::Predicate ModifyData::pred() const {
    return pred_;
  }

  int ModifyData::getID() {
    return id_;
  }
}  // namespace parse
