/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <cmath>
#include <memory>
#include "materialize/temptable.hpp"
#include "plan/plan.hpp"
#include "record/layout.hpp"
#include "record/schema.hpp"
#include "tx/transaction.hpp"

namespace materialize {
class MaterializePlan : public plan::Plan {
 public:
    MaterializePlan(tx::Transaction* transaction, std::shared_ptr<plan::Plan>& srcplan);
   std::shared_ptr<scan::Scan> open();
   int blocksAccessed();
   int recordsOutput();
   int distinctValues(const std::string& fldname);
   record::Schema schema();
 private:
    tx::Transaction* transaction_;
    std::shared_ptr<plan::Plan> srcplan_;
};
}  // namespace materialize
