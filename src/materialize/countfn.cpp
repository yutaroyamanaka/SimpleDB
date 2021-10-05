/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/countfn.hpp"

namespace materialize {
  CountFn::CountFn() {
    cnt_ = 0;
  }

  void CountFn::processFirst(scan::Scan* s) {
    cnt_ = 1;
  }

  void CountFn::processNext(scan::Scan* s) {
    cnt_++;
  }

  std::string CountFn::fieldName() {
    return parse::Word::COUNT;
  }

  scan::Constant CountFn::value() {
    scan::Constant c(cnt_);
    return c;
  }
}  // namespace materialize
