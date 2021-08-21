/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include "app/simpledb.hpp"
#include "plan/plan.hpp"
#include "plan/planner.hpp"
#include "scan/scan.hpp"
#include "tx/transaction.hpp"

TEST(PlannerTest2, Main) {
  std::string file_name = "plannerTest2";
  app::SimpleDB db(file_name);
  auto transaction = db.getNewTx();
  plan::Planner& planner = db.getPlanner();

  std::string cmd = "create table t1 ( a int, b varchar ( 12 ) )";
  planner.executeUpdate(cmd, transaction.get());

  for (int i = 0; i < 200; i++) {
    std::string b = "t1-" + std::to_string(i);
    cmd = "insert into t1 ( a, b ) values ( " +
      std::to_string(i) + ", '" + b + "' )";
    planner.executeUpdate(cmd, transaction.get());
  }

  cmd = "create table t2 ( c int, d varchar ( 12 ) )";
  planner.executeUpdate(cmd, transaction.get());

  for (int i = 0; i < 200; i++) {
    std::string b = "t2-" + std::to_string(200 - i - 1);
    cmd = "insert into t2 ( c, d ) values ( " +
      std::to_string(200 - i - 1) + ", '" + b + "' )";
    planner.executeUpdate(cmd, transaction.get());
  }

  std::string qry ="select b, d from t1,t2 where a = c";
  auto p = planner.createQueryPlan(qry, transaction.get());
  auto s = p->open();
  while (s->next()) {
    std::cout << s->getString("b") << " " << s->getString("d") << std::endl;
  }
  s->close();
  transaction->commit();
}
