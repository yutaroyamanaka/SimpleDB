/* Copyright 2021 Yutaro Yamanaka */
#pragma once
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
    virtual int executeInsert(const parse::InsertData& data, tx::Transaction* transaction) = 0;
    virtual int executeDelete(const parse::DeleteData& data, tx::Transaction* transaction) = 0;
    virtual int executeModify(const parse::ModifyData& data, tx::Transaction* transaction) = 0;
    virtual int executeCreateTable(const parse::CreateTableData& data, tx::Transaction* transaction) = 0;
    virtual int executeCreateView(const parse::CreateViewData& data, tx::Transaction* transaction) = 0;
    virtual int executeCreateIndex(const parse::CreateIndexData& data, tx::Transaction* transaction) = 0;
};
}  // namespace plan
