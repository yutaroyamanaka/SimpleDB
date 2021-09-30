/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/groupvalue.hpp"

namespace materialize {
  bool operator==(const GroupValue& lhs, const GroupValue& rhs) {
    for (const auto& [fldname, val] : lhs.vals_) {
      if (lhs.vals_.find(fldname) == lhs.vals_.end() || lhs.vals_.at(fldname) != val) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const GroupValue& lhs, const GroupValue& rhs) {
    if (lhs == rhs) return false;
    return true;
  }

  GroupValue::GroupValue(scan::Scan* s, const std::vector<std::string>& fields) {
    for (const auto& fldname : fields) {
      vals_[fldname] = s->getVal(fldname);
    }
  }

  scan::Constant GroupValue::getVal(const std::string& fldname) {
    return vals_.at(fldname);
  }

  int GroupValue::hashCode() {
    int hashval = 0;
    for (const auto& [key, val] : vals_) {
      hashval += val.hashCode();
    }
    return hashval;
  }
}  // namespace materialize
