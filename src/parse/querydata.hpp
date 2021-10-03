/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <set>
#include <string>
#include <vector>
#include "parse/word.hpp"
#include "scan/constant.hpp"
#include "scan/predicate.hpp"

namespace parse {
class QueryData {
 public:
    QueryData(const std::vector<std::string>& fields, const std::set<std::string>& tables, 
        const scan::Predicate& pred, const std::vector<std::string>& sortFields);
    std::vector<std::string> fields() const;
    std::set<std::string> tables() const;
    std::vector<QueryData> queryTables() const;
    void addQueryData(const QueryData& qd);
    scan::Predicate pred() const;
    std::vector<std::string> sortFields() const;
    std::string toString() const;
 private:
    std::vector<std::string> fields_;
    std::set<std::string> tables_;
    std::vector<QueryData> queryTables_;
    scan::Predicate pred_;
    std::vector<std::string> sortFields_;
};
}  // namespace parse
