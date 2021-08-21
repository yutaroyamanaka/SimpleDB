/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "parse/object.hpp"
#include "parse/objectid.hpp"

namespace parse {
class CreateIndexData : public Object {
 public:
    CreateIndexData(const std::string& idxname, const std::string& tblname, const std::string& fldname);
    std::string indexName() const;
    std::string tableName() const;
    std::string fieldName() const;

    int getID() override;
 private:
    std::string idxname_;
    std::string tblname_;
    std::string fldname_;
    const int id_ = ObjectID::CREATE_INDEX;
};
}  // namespace parse
