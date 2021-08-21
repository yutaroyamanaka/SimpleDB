/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "meta/statmanager.hpp"
#include "meta/metadatamanager.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "plan/plan.hpp"
#include "tx/transaction.hpp"

namespace plan {
class TablePlan : public Plan {
 public:
    TablePlan(tx::Transaction* transaction, const std::string& tblname, meta::MetaDataManager* mdm);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    tx::Transaction* transaction_;
    std::string tblname_;
    record::Layout layout_;
    meta::StatInfo si_;
};
}  // namespace plan
