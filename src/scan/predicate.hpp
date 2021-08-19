/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "plan/plan.hpp"
#include "scan/term.hpp"
#include "scan/scan.hpp"
#include "record/schema.hpp"

namespace scan {
class Predicate {
 public:
    Predicate();
    Predicate(const Term& t);
    Predicate(const Predicate& pred);
    Predicate &operator=(const Predicate& e);
    void conjoinWith(const Predicate& pred);
    bool isSatisfied(Scan* s) const;
    int reductionFactor(plan::Plan* p) const;
    Predicate selectSubPred(const record::Schema& sch) const;
    Predicate joinSubPred(const record::Schema& sch1, const record::Schema& sch2) const;
    Constant equatesWithConstant(const std::string& fldname) const;
    std::string equatesWithField(const std::string& fldname) const;
    std::string toString() const;
 private:
    std::vector<Term> terms_;
};
}  // namespace scan
