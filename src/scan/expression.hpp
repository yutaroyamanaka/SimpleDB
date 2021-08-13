/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "scan/constant.hpp"
#include "record/schema.hpp"
#include "scan/scan.hpp"

namespace scan {
class Expression {
 public:
    Expression();
    Expression(const Expression& e);
    Expression(const Constant& val);
    Expression(const std::string& fldname);
    Expression &operator=(const Expression& e);
    bool isFieldName() const;
    Constant asConstant() const;
    std::string asFieldName() const;
    Constant evaluate(Scan* s) const;
    bool appliesTo(const record::Schema& sch) const;
    std::string toString() const;
 private:
    Constant val_;
    std::string fldname_;
};
}  // namespace scan
