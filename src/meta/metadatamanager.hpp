/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include <map>
#include "tx/transaction.hpp"
#include "meta/tablemanager.hpp"
#include "meta/viewmanager.hpp"
#include "meta/statmanager.hpp"
#include "meta/indexmanager.hpp"
#include "record/schema.hpp"

namespace meta {
class MetaDataManager {
 public:
    MetaDataManager(bool isNew, tx::Transaction* transaction);
    void createTable(const std::string& tblname, const record::Schema& schema, tx::Transaction* transaction);
    record::Layout getLayout(const std::string& tblname, tx::Transaction* transaction);
    void createView(const std::string& viewname, const std::string& viewdef, tx::Transaction* transaction);
    std::string getViewDef(const std::string& viewname, tx::Transaction* transaction);
    void createIndex(const std::string& idxname, const std::string& tblname, const std::string& fldname, tx::Transaction* transaction);
    std::map<std::string, IndexInfo> getIndexInfo(const std::string& tblname, tx::Transaction* transaction);
    StatInfo getStatInfo(const std::string& tblname, const record::Layout& layout, tx::Transaction* transaction);
 private:
    static std::unique_ptr<TableManager> table_manager_;
    static std::unique_ptr<ViewManager> view_manager_;
    static std::unique_ptr<StatManager> stat_manager_;
    static std::unique_ptr<IndexManager> index_manager_;
};
}  // namespace meta
