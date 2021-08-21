/* Copyright 2021 Yutaro Yamanaka */
#include "scan/term.hpp"

namespace scan {
  Term::Term() {
  }

  Term::Term(const Expression& lhs, const Expression& rhs) : lhs_(lhs), rhs_(rhs) {
  }

  bool Term::isSatisfied(Scan* s) const {
    Constant lhsval = lhs_.evaluate(s);
    Constant rhsval = rhs_.evaluate(s);
    return lhsval == rhsval;
  }

  bool Term::appliesTo(const record::Schema& sch) const {
    return lhs_.appliesTo(sch) && rhs_.appliesTo(sch);
  }

  int Term::reductionFactor(plan::Plan* p) const {
    std::string lhsName, rhsName;
    if (lhs_.isFieldName() && rhs_.isFieldName()) {
      lhsName = lhs_.asFieldName();
      rhsName = rhs_.asFieldName();
      return std::max(p->distinctValues(lhsName), p->distinctValues(rhsName));
    }
    if (lhs_.isFieldName()) {
      lhsName = lhs_.asFieldName();
      return p->distinctValues(lhsName);
    }
    if (rhs_.isFieldName()) {
      rhsName = rhs_.asFieldName();
      return p->distinctValues(rhsName);
    }
    if (lhs_.asConstant() == rhs_.asConstant()) {
      return 1;
    }
    return INT_MAX;
  }

  Constant Term::equatesWithConstant(const std::string& fldname) const {
    if (lhs_.isFieldName() && lhs_.asFieldName() == fldname && !rhs_.isFieldName()) {
      return rhs_.asConstant();
    } else if (rhs_.isFieldName() && rhs_.asFieldName() == fldname && !lhs_.isFieldName()) {
      return lhs_.asConstant();
    } else {
      Constant c;
      return c;
    }
  }

  std::string Term::equatesWithField(const std::string& fldname) const {
    if (lhs_.isFieldName() && lhs_.asFieldName() == fldname && rhs_.isFieldName()) {
      return rhs_.asFieldName();
    } else if (rhs_.isFieldName() && rhs_.asFieldName() == fldname && lhs_.isFieldName()) {
      return lhs_.asFieldName();
    } else {
      return "";
    }
  }

  std::string Term::toString() const {
    return lhs_.toString() + "=" + rhs_.toString();
  }
}  // namespace scan
