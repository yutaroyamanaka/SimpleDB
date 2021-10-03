/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "materialize/materializeplan.hpp"
#include "materialize/recordcomparator.hpp"
#include "materialize/sortscan.hpp"
#include "materialize/temptable.hpp"
#include "plan/plan.hpp"
#include "scan/updatescan.hpp"
#include "record/schema.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"
#include "scan/scan.hpp"
#include "scan/updatescan.hpp"

namespace materialize {
class SortPlan : public plan::Plan {
 public:
    SortPlan(const std::shared_ptr<plan::Plan>& p, const std::vector<std::string>& sortFields, tx::Transaction* transaction);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
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
