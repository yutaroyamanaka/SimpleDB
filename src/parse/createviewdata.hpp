/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "parse/object.hpp"
#include "parse/querydata.hpp"

namespace parse {
class CreateViewData : public Object {
 public:
    CreateViewData(const std::string& viewname, const QueryData& qrydata);
    std::string viewName() const;
    std::string viewDef() const;
 private:
    std::string viewname_;
    QueryData qrydata_;
};
}  // namespace parse
