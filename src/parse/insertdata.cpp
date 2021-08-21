/* Copyright 2021 Yutaro Yamanaka */
#include "parse/insertdata.hpp"

namespace parse {
  InsertData::InsertData(const std::string& tblname,
      const std::vector<std::string>& flds, const std::vector<scan::Constant>& vals) : tblname_(tblname), flds_(flds), vals_(vals) {
  }

  std::string InsertData::tableName() const {
    return tblname_;
  }

  std::vector<std::string> InsertData::fields() const {
    return flds_;
  }

  std::vector<scan::Constant> InsertData::vals() const {
    return vals_;
  }

  int InsertData::getID() {
    return id_;
  }
}  // namespace parse
