/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <map>
#include <string>
#include <vector>
#include "scan/constant.hpp"
#include "scan/scan.hpp"

namespace materialize {
class GroupValue {
  friend bool operator==(const GroupValue& lhs, const GroupValue& rhs);
  friend bool operator!=(const GroupValue& lhs, const GroupValue& rhs);

 public:
   GroupValue(scan::Scan* s, const std::vector<std::string>& fields);
   scan::Constant getVal(const std::string& fldname);
   int hashCode();
 private:
   std::map<std::string, scan::Constant> vals_;
};
}  // namespace materialize
