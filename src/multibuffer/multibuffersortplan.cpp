/* Copyright 2021 Yutaro Yamanaka */
#include "multibuffer/multibuffersortplan.hpp"

namespace multibuffer {
  MultiBufferSortPlan::MultiBufferSortPlan(const std::shared_ptr<plan::Plan>& p, const std::vector<std::string>& sortFields, tx::Transaction* transaction)
    : p_(p), transaction_(transaction), comp_(sortFields) {
    sch_ = p->schema();
  }

  std::shared_ptr<scan::Scan> MultiBufferSortPlan::open() {
    int size = blocksAccessed();
    int available = transaction_->availableBuffs();
    int numbuffs = BufferNeeds::bestRoot(available, size);

    auto runs = splitIntoBlockWise(numbuffs);
    for (const auto& run : runs) {
      sortTempTable(run);
    }

    while (runs.size() > 1) {
      runs = doAMergeIteration(runs, numbuffs);
    }

    auto dest = runs[0]->open();
    return std::static_pointer_cast<scan::Scan>(dest);
  }

  int MultiBufferSortPlan::blocksAccessed() {
    // does not include the one-time cost of sorting
    auto mp = std::static_pointer_cast<plan::Plan>(std::make_shared<materialize::MaterializePlan>(transaction_, p_));
    return mp->blocksAccessed();
  }

  int MultiBufferSortPlan::recordsOutput() {
    return p_->recordsOutput();
  }

  int MultiBufferSortPlan::distinctValues(const std::string& fldname) {
    return p_->distinctValues(fldname);
  }

  record::Schema MultiBufferSortPlan::schema() {
    return sch_;
  }

  std::vector<std::shared_ptr<materialize::TempTable>> MultiBufferSortPlan::splitIntoBlockWise(int k) {
    std::vector<std::shared_ptr<materialize::TempTable>> temps;

    auto originTemp = std::make_shared<materialize::TempTable>(transaction_, sch_);
    auto src = p_->open();
    auto originScan = originTemp->open();
    while (src->next()) {
      originScan->insert();
      for (const auto& fldname : sch_.fields()) {
        originScan->setVal(fldname, src->getVal(fldname));
      }
    }
    src->close();
    originScan->beforeFirst();

    auto currentTemp = std::make_shared<materialize::TempTable>(transaction_, sch_);
    temps.emplace_back(currentTemp);
    auto currentScan = currentTemp->open();
    int prevBlockNum = -1;

    while (originScan->next()) {
      currentScan->insert();
      for (const auto& fldname : sch_.fields()) {
        currentScan->setVal(fldname, originScan->getVal(fldname));
      }

      int currentBlockNum = originScan->getRid().blockNumber();
      if (prevBlockNum != -1 && currentBlockNum == prevBlockNum + k) {
        currentScan->close();
        currentTemp = std::make_shared<materialize::TempTable>(transaction_, sch_);
        temps.emplace_back(currentTemp);
        currentScan = currentTemp->open();
      }
      prevBlockNum = currentBlockNum;
    }
    currentScan->close();
    return temps;
  }

  void MultiBufferSortPlan::sortTempTable(const std::shared_ptr<materialize::TempTable>& temp) {
    // excute in-memory selection sort
    auto exscan = temp->open();
    auto inscan1 = temp->open();
    auto inscan2 = temp->open();

    while (exscan->next()) {
      record::RID minValRID = exscan->getRid();
      inscan1->moveToRid(minValRID);

      while (inscan1->next()) {
        inscan2->moveToRid(minValRID);
        if (comp_.compare(inscan1.get(), inscan2.get()) < 0) {
          minValRID = inscan1->getRid();
        }
      }

      // swap
      inscan2->moveToRid(minValRID);
      for (const auto& fldname : sch_.fields()) {
        scan::Constant tmpConst = inscan2->getVal(fldname);
        inscan2->setVal(fldname, exscan->getVal(fldname));
        exscan->setVal(fldname, tmpConst);
      }
      inscan1->beforeFirst();
    }

    exscan->close();
    inscan1->close();
    inscan2->close();
  }

  std::vector<std::shared_ptr<materialize::TempTable>> MultiBufferSortPlan::
    doAMergeIteration(const std::vector<std::shared_ptr<materialize::TempTable>>& runs, int k) {
    std::vector<std::shared_ptr<materialize::TempTable>> result;
    int i = 0;

    for (i = 0; i + k <= runs.size(); i+=k) {
      std::vector<std::shared_ptr<materialize::TempTable>> temps;
      for (int j = 0; j < k; j++) {
        temps.emplace_back(runs[i+j]);
      }
      result.emplace_back(mergeSeveralRuns(temps));
    }

    while (i < runs.size()) {
      result.emplace_back(runs[i]);
      i++;
    }
    return result;
  }

  std::shared_ptr<materialize::TempTable> MultiBufferSortPlan::mergeSeveralRuns(const std::vector<std::shared_ptr<materialize::TempTable>>& temps) {
    auto result = std::make_shared<materialize::TempTable>(transaction_, sch_);
    auto dest = result->open();

    std::vector<std::shared_ptr<scan::UpdateScan>> scans;
    std::vector<bool> hasmores;

    int i = 0;
    for (const auto& temp : temps) {
      scans.emplace_back(temp->open());
      hasmores.emplace_back(scans[i]->next());
      i++;
    }

    while (true) {
      std::vector<std::shared_ptr<scan::Scan>> currentScans;
      std::vector<int> currentIndexs;

      for (int i = 0; i < hasmores.size(); i++) {
        if (hasmores[i]) {
          currentScans.emplace_back(std::static_pointer_cast<scan::Scan>(scans[i]));
          currentIndexs.emplace_back(i);
        }
      }

      if (currentScans.size() == 0) break;
      int mini = comp_.minIndex(currentScans);
      hasmores[currentIndexs[mini]] = copy(currentScans[mini].get(), dest.get());
    }

    for (const auto& scan : scans) {
      scan->close();
    }
    return result;
  }

  bool MultiBufferSortPlan::copy(scan::Scan* src, scan::UpdateScan* dest) {
    dest->insert();
    for (const auto& fldname : sch_.fields()) {
      dest->setVal(fldname, src->getVal(fldname));
    }
    return src->next();
  }
}  // namespace multibuffer
