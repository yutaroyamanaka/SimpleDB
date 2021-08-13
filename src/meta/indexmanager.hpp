/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <map>
#include "tx/transaction.hpp"
#include "record/schema.hpp"
#include "record/layout.hpp"
#include "meta/statmanager.hpp"

namespace meta {
class IndexInfo {
 public:
    IndexInfo();
    IndexInfo(const std::string& idxname, const std::string& fldname, const record::Schema& schema, tx::Transaction* transaction, const StatInfo& si);
    // Index open();
    void open();
    int blocksAccessed();
    int recordOutput();
    int distinctValues(const std::string& fname);
 private:
    std::string idxname_, fldname_;
    tx::Transaction* transaction_;
    record::Schema schema_;
    record::Layout layout_;
    StatInfo si_;
    record::Layout createIdxLayout();
};

class IndexManager {
 public:
    IndexManager(bool isNew, TableManager* table_manager, StatManager* stat_manager, tx::Transaction* transaction);
    void createIndex(const std::string& idxname, const std::string& tblname, const std::string& fldname, tx::Transaction* transaction);
    std::map<std::string, IndexInfo> getIndexInfo(const std::string& tblname, tx::Transaction* transaction);
 private:
    record::Layout layout_;
    TableManager* table_manager_;
    StatManager* stat_manager_;
};
}  // namespace meta
