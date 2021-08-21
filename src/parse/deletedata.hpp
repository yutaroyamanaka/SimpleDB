/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "parse/object.hpp"
#include "parse/objectid.hpp"
#include "scan/predicate.hpp"

namespace parse {
class DeleteData : public Object {
 public:
    DeleteData(const std::string& tblname, const scan::Predicate& pred);
    std::string tableName() const;
    scan::Predicate pred() const;

    int getID() override;
 private:
    std::string tblname_;
    scan::Predicate pred_;
    const int id_ = ObjectID::REMOVE;
};
}  // namespace parse
