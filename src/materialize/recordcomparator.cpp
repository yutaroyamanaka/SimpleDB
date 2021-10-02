/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/recordcomparator.hpp"

namespace materialize {
  RecordComparator::RecordComparator(const std::vector<std::string>& fields) : fields_(fields) {
  }

  int RecordComparator::compare(scan::Scan* s1, scan::Scan* s2) {
    for (const auto& fldname : fields_) {
      scan::Constant val1 = s1->getVal(fldname);
      scan::Constant val2 = s2->getVal(fldname);
      int result = val1 == val2;
      if (result != 0) return result;
    }
    return 0;
  }
}  // namespace materialize
