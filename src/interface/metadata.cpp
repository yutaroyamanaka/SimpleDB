/* Copyright 2021 Yutaro Yamanaka */
#include "interface/metadata.hpp"

namespace interface {
  MetaData::MetaData(const record::Schema& sch) : sch_(sch) {
  }

  int MetaData::getColumnCount() {
    try {
      return sch_.fields().size();
    } catch (std::exception& e) {
      throw std::runtime_error("cannot execute MetaData::getColumnCount");
    }
  }

  std::string MetaData::getColumnName(int column) {
    try {
      return sch_.fields()[column - 1];
    } catch (std::exception& e) {
      throw std::runtime_error("cannot execute MetaData::getColumnName");
    }
  }

  int MetaData::getColumnType(int column) {
    try {
      std::string fldname = getColumnName(column);
      return sch_.type(fldname);
    } catch (std::exception& e) {
      throw std::runtime_error("cannot execute MetaData::getColumnType");
    }
  }

  int MetaData::getColumnDisplaySize(int column) {
    try {
      std::string fldname = getColumnName(column);
      int fldtype = sch_.type(fldname);
      int fldlength = (fldtype == record::Schema::INTEGER) ? 6 : sch_.length(fldname);
      return fldname.length() > fldlength ? fldname.length() + 1 : fldlength + 1;
    } catch (std::exception& e) {
      throw std::runtime_error("cannot execute MetaData::getColumnDisplaySize");
    }
  }
}  // namespace interface
