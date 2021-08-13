/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "record/schema.hpp"
#include "record/layout.hpp"
#include "tx/transaction.hpp"

namespace meta {
class TableManager {
 public:
    TableManager(bool isNew, tx::Transaction* transaction);
    void createTable(const std::string& tblname, const record::Schema& schema, tx::Transaction* transaction);
    record::Layout getLayout(const std::string& tblname, tx::Transaction* transaction);
    const static int MAX_NAME = 16;
 private:
    std::unique_ptr<record::Layout> tcat_layout_, fcat_layout_;
};
}  // namespace meta
