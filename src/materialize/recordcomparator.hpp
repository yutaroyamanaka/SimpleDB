/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <vector>
#include "scan/constant.hpp"
#include "scan/scan.hpp"

namespace materialize {
class RecordComparator {
 public:
    RecordComparator(const std::vector<std::string>& fields);
    int compare(scan::Scan* s1, scan::Scan* s2);
    int minIndex(const std::vector<std::shared_ptr<scan::Scan>>& scans);
 private:
    std::vector<std::string> fields_;
};
}  // namespace materialize
