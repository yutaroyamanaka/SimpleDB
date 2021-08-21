/* Copyright 2021 Yutaro Yamanaka */
#include "parse/createviewdata.hpp"

namespace parse {
  CreateViewData::CreateViewData(const std::string& viewname, const QueryData& qrydata) : viewname_(viewname), qrydata_(qrydata) {
  }

  std::string CreateViewData::viewName() const {
    return viewname_;
  }

  std::string CreateViewData::viewDef() const {
    return qrydata_.toString();
  }

  int CreateViewData::getID() {
    return id_;
  }
}  // namespace parse
