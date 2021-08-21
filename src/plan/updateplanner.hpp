/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include "parse/createindexdata.hpp"
#include "parse/createtabledata.hpp"
#include "parse/createviewdata.hpp"
#include "parse/deletedata.hpp"
#include "parse/insertdata.hpp"
#include "parse/modifydata.hpp"
#include "plan/plan.hpp"
#include "tx/transaction.hpp"

namespace plan {
class UpdatePlanner {
 public:
    ~UpdatePlanner() = default;
    virtual int executeInsert(parse::InsertData* data, tx::Transaction* transaction) = 0;
    virtual int executeDelete(parse::DeleteData* data, tx::Transaction* transaction) = 0;
    virtual int executeModify(parse::ModifyData* data, tx::Transaction* transaction) = 0;
    virtual int executeCreateTable(parse::CreateTableData* data, tx::Transaction* transaction) = 0;
    virtual int executeCreateView(parse::CreateViewData* data, tx::Transaction* transaction) = 0;
    virtual int executeCreateIndex(parse::CreateIndexData* data, tx::Transaction* transaction) = 0;
};
}  // namespace plan
