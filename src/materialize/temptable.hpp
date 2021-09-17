/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <mutex>
#include <string>
#include "record/layout.hpp"
#include "record/schema.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"
#include "scan/updatescan.hpp"

namespace materialize {
class TempTable {
 public:
   TempTable(tx::Transaction* transaction, const record::Schema& sch);
   std::shared_ptr<scan::UpdateScan> open();
   std::string tableName();
   record::Layout getLayout();
   static std::string nextTableName();
 private:
    static std::mutex mutex_;
    static int nextTableNum_;
    tx::Transaction* transaction_;
    std::string tblname_;
    record::Layout layout_;
};
}  // namespace materialize
