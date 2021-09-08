/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "indexing/btreeindex.hpp"
#include "meta/metadatamanager.hpp"
#include "plan/plan.hpp"
#include "plan/selectplan.hpp"
#include "plan/updateplanner.hpp"
#include "plan/tableplan.hpp"
#include "tx/transaction.hpp"
#include "scan/constant.hpp"
#include "scan/updatescan.hpp"

namespace indexing {
class IndexUpdatePlanner : public plan::UpdatePlanner {
 public:
    IndexUpdatePlanner(meta::MetaDataManager* mdm);
    int executeDelete(parse::DeleteData* data, tx::Transaction* transaction) override;
    int executeModify(parse::ModifyData* data, tx::Transaction* transaction) override;
    int executeInsert(parse::InsertData* data, tx::Transaction* transaction) override;
    int executeCreateTable(parse::CreateTableData* data, tx::Transaction* transaction) override;
    int executeCreateView(parse::CreateViewData* data, tx::Transaction* transaction) override;
    int executeCreateIndex(parse::CreateIndexData* data, tx::Transaction* transaction) override;
 private:
    meta::MetaDataManager* mdm_;
};
}  // namespace indexing
