/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/temptable.hpp"

namespace materialize {
  int TempTable::nextTableNum_ = 0;
  std::mutex TempTable::mutex_;

  TempTable::TempTable(tx::Transaction* transaction, const record::Schema& sch) : transaction_(transaction) {
    tblname_ = nextTableName();
    layout_ = record::Layout(sch);
  }

  std::shared_ptr<scan::UpdateScan> TempTable::open() {
    return std::static_pointer_cast<scan::UpdateScan>(std::make_shared<record::TableScan>(transaction_, tblname_, layout_));
  }

  std::string TempTable::tableName() {
    return tblname_;
  }

  record::Layout TempTable::getLayout() {
    return layout_;
  }

  std::string TempTable::nextTableName() {
    std::unique_lock<std::mutex> lock(mutex_);
    nextTableNum_++;
    return "temp" + std::to_string(nextTableNum_);
  }
}  // namespace materialize
