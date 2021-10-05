/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/maxfn.hpp"

namespace materialize {
  MaxFn::MaxFn(const std::string& fldname) : fldname_(fldname) {
  }

  void MaxFn::processFirst(scan::Scan* s) {
    val_ = s->getVal(fldname_);
  }

  void MaxFn::processNext(scan::Scan* s) {
    scan::Constant newval = s->getVal(fldname_);
    if (newval > val_) {
      val_ = newval;
    }
  }

  std::string MaxFn::fieldName() {
    return parse::Word::MAX + fldname_;
  }

  scan::Constant MaxFn::value() {
    return val_;
  }
}  // namespace materialize
