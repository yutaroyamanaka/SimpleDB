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
#include "record/rid.hpp"
#include "tx/transaction.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"
#include "scan/updatescan.hpp"

namespace materialize {
class SortScan : public scan::Scan {
 public:
    SortScan(const std::vector<std::shared_ptr<TempTable>>& runs, const RecordComparator& comp);
    void beforeFirst();
    bool next();
    void close();
    scan::Constant getVal(const std::string& fldname);
    int getInt(const std::string& fldname);
    std::string getString(const std::string& fldname);
    bool hasField(const std::string& fldname);
    void savePosition();
    void restorePosition();
 private:
    std::shared_ptr<scan::UpdateScan> s1_, s2_, currentScan_;
    RecordComparator comp_;
    bool hasmore1_ = false;
    bool hasmore2_ = false;
    std::vector<record::RID> savedPosition_;
};
}  // namespace materialize
