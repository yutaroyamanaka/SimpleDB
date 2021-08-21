/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "parse/object.hpp"
#include "parse/objectid.hpp"
#include "scan/expression.hpp"
#include "scan/predicate.hpp"

namespace parse {
class ModifyData : public Object {
 public:
    ModifyData(const std::string& tblname, const std::string& fldname, const scan::Expression& newval, const scan::Predicate& pred);
    std::string tableName() const;
    std::string targetField() const;
    scan::Expression newValue() const;
    scan::Predicate pred() const;

    int getID() override;
 private:
    std::string tblname_;
    std::string fldname_;
    scan::Expression newval_;
    scan::Predicate pred_;

    const int id_ = ObjectID::MODIFY;
};
}  // namespace parse
