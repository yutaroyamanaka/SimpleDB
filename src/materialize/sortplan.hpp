/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/temptable.hpp"
#include "materialize/recordcomparator.hpp"
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"
#include "scan/scan.hpp"
#include "scan/updatescan.hpp"

namespace materialize {
class SortPlan {
 public:
   SortPlan(std::shared_ptr<plan::Plan>& p, const std::vector<std::string>& sortFields, tx::Transaction* transaction);
   std::shared_ptr<scan::Scan> open();
   int blocksAccessed();
   int recordsOutput();
   int distinctValues(const std::string& fldname);
   record::Schema schema();
 private:
    tx::Transaction* transaction_;
    std::shared_ptr<plan::Plan> p_;
    record::Schema sch_;
    RecordComparator comp_;

    std::vector<std::shared_ptr<TempTable>> splitIntoRuns(scan::Scan* src);
    std::vector<std::shared_ptr<TempTable>> doAMergeIteration(const std::vector<std::shared_ptr<TempTable>>& runs);
    std::shared_ptr<TempTable> mergeTwoRuns(TempTable* p1, TempTable* p2);
    bool copy(scan::Scan* src, scan::UpdateScan* dest);
};
}  // namespace materialize
