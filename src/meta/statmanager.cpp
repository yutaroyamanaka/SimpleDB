/* Copyright 2021 Yutaro Yamanaka */
#include "meta/statmanager.hpp"

namespace meta {

  StatManager::StatManager(TableManager* table_manager, tx::Transaction* transaction) : table_manager_(table_manager) {
    refreshStatistics(transaction);
  }

  StatInfo StatManager::getStatInfo(const std::string& tblname, const record::Layout& layout, tx::Transaction* transaction) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    num_calls_++;
    if (num_calls_ > 100) {
      refreshStatistics(transaction);
    }

    auto itr = table_stats_.find(tblname);
    if (itr == table_stats_.end()) {
      StatInfo si = calcTableStats(tblname, layout, transaction);
      table_stats_[tblname] = si;
      return si;
    }

    StatInfo si = table_stats_.at(tblname);
    return si;
  }

  void StatManager::refreshStatistics(tx::Transaction* transaction) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    table_stats_.clear();
    record::Layout tcatlayout = table_manager_->getLayout("tblcat", transaction);
    record::TableScan tcat(transaction, "tblcat", tcatlayout);
    while (tcat.next()) {
      std::string tblname = tcat.getString("tblname");
      std::cout << tblname << std::endl;
      record::Layout layout = table_manager_->getLayout(tblname, transaction);
      StatInfo si = calcTableStats(tblname, layout, transaction);
      table_stats_[tblname] = si;
    }
    tcat.close();
  }

  StatInfo StatManager::calcTableStats(const std::string& tblname, const record::Layout& layout, tx::Transaction* transaction) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    int num_recs = 0;
    int num_blocks = 0;
    record::TableScan ts(transaction, tblname, layout);
    while (ts.next()) {
      num_recs++;
      num_blocks = ts.getRid().blockNumber() + 1;
    }
    ts.close();
    return StatInfo(num_blocks, num_recs);
  }

  StatInfo::StatInfo() {
  }

  StatInfo::StatInfo(int num_blocks, int num_recs) : num_blocks_(num_blocks), num_recs_(num_recs) {
  }

  int StatInfo::blocksAccessed() const {
    return num_blocks_;
  }

  int StatInfo::recordOutput() const {
    return num_recs_;
  }

  int StatInfo::distinctValues(const std::string& fldname) const {
    return 1 + (num_recs_ / 3);
  }
}  // namespace meta
