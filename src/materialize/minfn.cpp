/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/minfn.hpp"

namespace materialize {
  MinFn::MinFn(const std::string& fldname) : fldname_(fldname) {
  }

  void MinFn::processFirst(scan::Scan* s) {
    val_ = s->getVal(fldname_);
  }

  void MinFn::processNext(scan::Scan* s) {
    scan::Constant newval = s->getVal(fldname_);
    if (newval < val_) {
      val_ = newval;
    }
  }

  std::string MinFn::fieldName() {
    return parse::Word::MIN + fldname_;
  }

  scan::Constant MinFn::value() {
    return val_;
  }
}  // namespace materialize
