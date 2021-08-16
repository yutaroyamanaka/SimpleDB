/* Copyright 2021 Yutaro Yamanaka */
#include <string>
#include "parse/object.hpp"
#include "record/schema.hpp"

namespace parse {
class CreateTableData : public Object {
 public:
    CreateTableData(const std::string& tblname, const record::Schema& sch);
    std::string tableName() const;
    record::Schema newSchema() const;
 private:
    std::string tblname_;
    record::Schema sch_;
};
}  // namespace parse
