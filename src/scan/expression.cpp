/* Copyright 2021 Yutaro Yamanaka */
#include "scan/expression.hpp"

namespace scan {
  Expression::Expression() {
  }

  Expression::Expression(const Expression& e) : val_(e.val_), fldname_(e.fldname_) {
  }

  Expression::Expression(const Constant& val) : val_(val) {
  }

  Expression::Expression(const std::string& fldname) : fldname_(fldname) {
  }

  Expression &Expression::operator=(const Expression& e) {
    if (this != &e) {
      val_ = e.val_;
      fldname_ = e.fldname_;
    }
    return *this;
  }

  bool Expression::isFieldName() const {
    return !fldname_.empty();
  }

  Constant Expression::asConstant() const {
    return val_;
  }

  std::string Expression::asFieldName() const {
    return fldname_;
  }

  Constant Expression::evaluate(Scan* s) const {
    return val_.isNull() ? s->getVal(fldname_) : val_;
  }

  bool Expression::appliesTo(const record::Schema& sch) const {
    return val_.isNull() ? sch.hasField(fldname_) : true;
  }

  std::string Expression::toString() const {
    return val_.isNull() ? fldname_ : val_.toString();
  }
}  // namespace scan
