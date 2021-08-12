/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include "tablemanager.hpp"
#include "tablescan.hpp"
#include <map>
#include <string>
#include <mutex>

namespace meta {
  class StatInfo {
    public:
      StatInfo();
      StatInfo(int num_blocks, int num_recs);
      int blocksAccessed() const;
      int recordOutput() const;
      int distinctValues(const std::string& fldname) const;
    private:
      int num_blocks_, num_recs_;
  };

  class StatManager {
    public:
      StatManager(TableManager* table_manager, tx::Transaction* transaction);
      StatInfo getStatInfo(const std::string& tblname, const record::Layout& layout, tx::Transaction* transaction);
      void refreshStatistics(tx::Transaction* transaction);
      StatInfo calcTableStats(const std::string& tblname, const record::Layout& layout, tx::Transaction* transaction);
    private:
      TableManager* table_manager_;
      std::map<std::string, StatInfo> table_stats_;
      int num_calls_;
      std::recursive_mutex mutex_;
  };
}
