/* Copyright 2021 Yutaro Yamanaka */
#include "parse/querydata.hpp"

namespace parse {
  QueryData::QueryData(const std::vector<std::string>& fields, const std::set<std::string>& tables, const scan::Predicate& pred)
    : fields_(fields), tables_(tables), pred_(pred) {
  }

  std::vector<std::string> QueryData::fields() const {
    return fields_;
  }

  std::set<std::string> QueryData::tables() const {
    return tables_;
  }

  std::vector<QueryData> QueryData::queryTables() const {
    return queryTables_;
  }

  void QueryData::addQueryData(const QueryData& qd) {
    queryTables_.emplace_back(qd);
  }

  scan::Predicate QueryData::pred() const {
    return pred_;
  }

  std::string QueryData::toString() const {
    std::string result = Word::SELECT + Word::SPACE;
    for (const auto& fldname : fields_) {
      result += fldname + Word::COMMA + Word::SPACE;
    }
    result = result.substr(0, result.size()-2);
    result += Word::SPACE + Word::FROM + Word::SPACE;
    for (const auto& tblname : tables_) {
      result += tblname + Word::COMMA + Word::SPACE;
    }
    for (const auto& qt : queryTables_) {
      result += Word::LEFT_PARENTHESIS + Word::SPACE;
      result += qt.toString();
      result += Word::SPACE + Word::RIGHT_PARENTHESIS + Word::COMMA + Word::SPACE;
    }
    result = result.substr(0, result.size()-2);
    std::string predstring = pred_.toString();
    if (predstring != "") {
      result += Word::SPACE + Word::WHERE + Word::SPACE + predstring;
    }
    return result;
  }
}  // namespace parse
