/* Copyright 2021 Yutaro Yamanaka */
#include "metadatamanager.hpp"
#include <iostream>

namespace meta {
  std::unique_ptr<TableManager> MetaDataManager::table_manager_;
  std::unique_ptr<ViewManager> MetaDataManager::view_manager_;
  std::unique_ptr<StatManager> MetaDataManager::stat_manager_;
  std::unique_ptr<IndexManager> MetaDataManager::index_manager_;

  MetaDataManager::MetaDataManager(bool isNew, tx::Transaction* transaction) {
    table_manager_ = std::make_unique<TableManager>(isNew, transaction);
    view_manager_ = std::make_unique<ViewManager>(isNew, table_manager_.get(), transaction);
    stat_manager_ = std::make_unique<StatManager>(table_manager_.get(), transaction);
    index_manager_ = std::make_unique<IndexManager>(isNew, table_manager_.get(), stat_manager_.get(), transaction);
  }

  void MetaDataManager::createTable(const std::string& tblname, const record::Schema& sch, tx::Transaction* transaction) {
    table_manager_->createTable(tblname, sch, transaction);
  }

  record::Layout MetaDataManager::getLayout(const std::string& tblname, tx::Transaction* transaction) {
    return table_manager_->getLayout(tblname, transaction);
  }

  void MetaDataManager::createView(const std::string& viewname, const std::string& viewdef, tx::Transaction* transaction) {
    view_manager_->createView(viewname, viewdef, transaction);
  }

  std::string MetaDataManager::getViewDef(const std::string& viewname, tx::Transaction* transaction) {
    return view_manager_->getViewDef(viewname, transaction);
  }

  void MetaDataManager::createIndex(const std::string& idxname, const std::string& tblname, const std::string& fldname, tx::Transaction* transaction) {
    index_manager_->createIndex(idxname, tblname, fldname, transaction);
  }

  std::map<std::string, IndexInfo> MetaDataManager::getIndexInfo(const std::string& tblname, tx::Transaction* transaction) {
    return index_manager_->getIndexInfo(tblname, transaction);
  }

  StatInfo MetaDataManager::getStatInfo(const std::string& tblname, const record::Layout& layout, tx::Transaction* transaction) {
    return stat_manager_->getStatInfo(tblname, layout, transaction);
  }
}  // namespace meta
