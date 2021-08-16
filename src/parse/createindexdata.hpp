/* Copyright 2021 Yutaro Yamanaka */
#include <string>
#include "parse/object.hpp"

namespace parse {
class CreateIndexData : public Object {
 public:
    CreateIndexData(const std::string& idxname, const std::string& tblname, const std::string& fldname);
    std::string indexName() const;
    std::string tableName() const;
    std::string fieldName() const;
 private:
    std::string idxname_;
    std::string tblname_;
    std::string fldname_;
};
}  // namespace parse
