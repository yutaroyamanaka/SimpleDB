/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/sortplan.hpp"

namespace materialize {
  SortPlan::SortPlan(const std::shared_ptr<plan::Plan>& p, const std::vector<std::string>& sortFields, tx::Transaction* transaction)
    : p_(p), transaction_(transaction), comp_(sortFields) {
    sch_ = p->schema();
  }

  std::shared_ptr<scan::Scan> SortPlan::open() {
    auto src = p_->open();
    auto runs = splitIntoRuns(src.get());
    src->close();
    while (runs.size() > 2) {
      runs = doAMergeIteration(runs);
    }
    return std::static_pointer_cast<scan::Scan>(std::make_shared<SortScan>(runs, comp_));
  }

  int SortPlan::blocksAccessed() {
    // does not include the one-time cost of sorting
    auto mp = std::static_pointer_cast<plan::Plan>(std::make_shared<MaterializePlan>(transaction_, p_));
    return mp->blocksAccessed();
  }

  int SortPlan::recordsOutput() {
    return p_->recordsOutput();
  }

  int SortPlan::distinctValues(const std::string& fldname) {
    return p_->distinctValues(fldname);
  }

  record::Schema SortPlan::schema() {
    return sch_;
  }

  std::vector<std::shared_ptr<TempTable>> SortPlan::splitIntoRuns(scan::Scan* src) {
    std::vector<std::shared_ptr<TempTable>> temps;
    src->beforeFirst();
    if (!src->next()) {
      return temps;
    }

    auto currentTemp = std::make_shared<TempTable>(transaction_, sch_);
    temps.emplace_back(currentTemp);
    auto currentScan = currentTemp->open();
    while (copy(src, currentScan.get())) {
      if (comp_.compare(src, currentScan.get()) < 0) {
        // start a new run
        currentScan->close();
        currentTemp = std::make_shared<TempTable>(transaction_, sch_);
        temps.emplace_back(currentTemp);
        currentScan = std::static_pointer_cast<scan::UpdateScan>(currentTemp->open());
      }
    }
    currentScan->close();
    return temps;
  }

  std::vector<std::shared_ptr<TempTable>> SortPlan::doAMergeIteration(const std::vector<std::shared_ptr<TempTable>>& runs) {
    std::vector<std::shared_ptr<TempTable>> result;
    for (int i = 0; i + 2 <= runs.size(); i+=2) {
      auto p1 = runs[i];
      auto p2 = runs[i+1];
      result.emplace_back(mergeTwoRuns(p1.get(), p2.get()));
    }
    if (runs.size() % 2 == 1) {
      result.emplace_back(runs.back());
    }
    return result;
  }

  std::shared_ptr<TempTable> SortPlan::mergeTwoRuns(TempTable* p1, TempTable* p2) {
    auto src1 = p1->open();
    auto src2 = p2->open();
    auto result = std::make_shared<TempTable>(transaction_, sch_);
    auto dest = result->open();

    bool hasmore1 = src1->next();
    bool hasmore2 = src2->next();
    while (hasmore1 && hasmore2) {
      if (comp_.compare(src1.get(), src2.get()) < 0) {
        hasmore1 = copy(src1.get(), dest.get());
      } else {
        hasmore2 = copy(src2.get(), dest.get());
      }
    }

    if (hasmore1) {
      while (hasmore1) {
        hasmore1 = copy(src1.get(), dest.get());
      }
    } else {
      while (hasmore2) {
        hasmore2 = copy(src2.get(), dest.get());
      }
    }

    src1->close();
    src2->close();
    dest->close();
    return result;
  }

  bool SortPlan::copy(scan::Scan* src, scan::UpdateScan* dest) {
    dest->insert();
    for (const auto& fldname : sch_.fields()) {
      std::cout << fldname << " "  << src->getVal(fldname).toString() << " ";
      dest->setVal(fldname, src->getVal(fldname));
    }
    std::cout << std::endl;
    return src->next();
  }
}  // namespace materialize
