/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/materializeplan.hpp"
#include "materialize/recordcomparator.hpp"
#include "materialize/sortscan.hpp"
#include "materialize/temptable.hpp"
#include "multibuffer/bufferneeds.hpp"
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "record/tablescan.hpp"
#include "scan/scan.hpp"
#include "scan/updatescan.hpp"
#include "tx/transaction.hpp"

namespace multibuffer {
class MultiBufferSortPlan : public plan::Plan {
 public:
    MultiBufferSortPlan(const std::shared_ptr<plan::Plan>& p, const std::vector<std::string>& sortFields, tx::Transaction* transaction);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    tx::Transaction* transaction_;
    std::shared_ptr<plan::Plan> p_;
    record::Schema sch_;
    materialize::RecordComparator comp_;

    void sortTempTable(const std::shared_ptr<materialize::TempTable>& temp);
    std::vector<std::shared_ptr<materialize::TempTable>> splitIntoBlockWise(int k);
    std::vector<std::shared_ptr<materialize::TempTable>> doAMergeIteration(const std::vector<std::shared_ptr<materialize::TempTable>>& runs, int k);
    std::shared_ptr<materialize::TempTable> mergeSeveralRuns(const std::vector<std::shared_ptr<materialize::TempTable>>& temps);
    bool copy(scan::Scan* src, scan::UpdateScan* dest);
};
}  // namespace multibuffer
