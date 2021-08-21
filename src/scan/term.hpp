/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <climits>
#include <string>
#include "plan/plan.hpp"
#include "scan/expression.hpp"

namespace scan {
class Term {
 public:
    Term();
    Term(const Expression& lhs, const Expression& rhs);
    bool isSatisfied(Scan* s) const;
    bool appliesTo(const record::Schema& sch) const;
    int reductionFactor(plan::Plan* p) const;
    Constant equatesWithConstant(const std::string& fldname) const;
    std::string equatesWithField(const std::string& fldname) const;
    std::string toString() const;
 private:
    Expression lhs_, rhs_;
};
}  // namespace scan
