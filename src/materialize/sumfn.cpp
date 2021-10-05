/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/sumfn.hpp"

namespace materialize {
  SumFn::SumFn(const std::string& fldname) : fldname_(fldname) {
  }

  void SumFn::processFirst(scan::Scan* s) {
    val_ = s->getVal(fldname_);
  }

  void SumFn::processNext(scan::Scan* s) {
    scan::Constant newval = s->getVal(fldname_);
    val_ += newval;
  }

  std::string SumFn::fieldName() {
    return parse::Word::SUM + fldname_;
  }

  scan::Constant SumFn::value() {
    return val_;
  }
}  // namespace materialize
