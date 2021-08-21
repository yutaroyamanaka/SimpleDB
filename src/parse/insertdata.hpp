/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <vector>
#include "parse/object.hpp"
#include "parse/objectid.hpp"
#include "scan/constant.hpp"

namespace parse {
class InsertData : public Object {
 public:
    InsertData(const std::string& tblname, const std::vector<std::string>& flds,
        const std::vector<scan::Constant>& vals);
    std::string tableName() const;
    std::vector<std::string> fields() const;
    std::vector<scan::Constant> vals() const;

    int getID() override;
 private:
    std::string tblname_;
    std::vector<std::string> flds_;
    std::vector<scan::Constant> vals_;

    const int id_ = ObjectID::INSERT;
};
}  // namespace parse
