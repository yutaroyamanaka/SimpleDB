/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "app/simpledb.hpp"
#include "interface/statement.hpp"
#include "plan/planner.hpp"
#include "tx/transaction.hpp"

namespace interface {
class Statement;

class Connection {
 public:
   Connection(std::unique_ptr<app::SimpleDB> db);
   std::unique_ptr<Statement> createStatement();
   void close();
   void commit();
   void rollback();
   tx::Transaction& getTransaction();
 private:
   std::unique_ptr<app::SimpleDB> db_;
   std::unique_ptr<tx::Transaction> transaction_;
   plan::Planner& planner_;
};
}  // namespace interface
