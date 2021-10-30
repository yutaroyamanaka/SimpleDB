/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "materialize/materializeplan.hpp"
#include "materialize/temptable.hpp"
#include "multibuffer/bufferneeds.hpp"
#include "multibuffer/multibufferproductscan.hpp"
#include "plan/plan.hpp"
#include "record/schema.hpp"
#include "record/tablescan.hpp"
#include "scan/scan.hpp"
#include "scan/updatescan.hpp"
#include "tx/transaction.hpp"

namespace multibuffer {
class MultiBufferProductPlan : public plan::Plan {
 public:
    MultiBufferProductPlan(const std::shared_ptr<plan::Plan>& lhs, const std::shared_ptr<plan::Plan>& rhs, tx::Transaction* transaction);
    std::shared_ptr<scan::Scan> open() override;
    int blocksAccessed() override;
    int recordsOutput() override;
    int distinctValues(const std::string& fldname) override;
    record::Schema schema() override;
 private:
    tx::Transaction* transaction_;
    std::shared_ptr<plan::Plan> lhs_, rhs_;
    record::Schema sch_;

    std::shared_ptr<materialize::TempTable> copyRecordsFrom(const std::shared_ptr<plan::Plan>& p);
};
}  // namespace multibuffer
