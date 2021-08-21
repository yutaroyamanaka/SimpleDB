/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "parse/object.hpp"
#include "parse/objectid.hpp"
#include "record/schema.hpp"

namespace parse {
class CreateTableData : public Object {
 public:
    CreateTableData(const std::string& tblname, const record::Schema& sch);
    std::string tableName() const;
    record::Schema newSchema() const;

    int getID() override;
 private:
    std::string tblname_;
    record::Schema sch_;
    const int id_ = ObjectID::CREATE_TABLE;
};
}  // namespace parse
