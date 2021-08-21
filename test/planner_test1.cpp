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

TEST(PlannerTest1, Main) {
  std::string file_name = "plannerTest1";
  app::SimpleDB db(file_name);
  auto transaction = db.getNewTx();
  plan::Planner& planner = db.getPlanner();
  std::string cmd = "create table student ( id int, name varchar ( 12 ) )";

  planner.executeUpdate(cmd, transaction.get());

  std::cout << "student" << std::endl;
  for (int i = 0; i < 200; i++) {
    std::string b = "rec" + std::to_string(i % 10);
    cmd = "insert into student ( id, name ) values ( " +
      std::to_string(i) + ", '" + b + "' )";
    planner.executeUpdate(cmd, transaction.get());
  }

  std::string qry ="select id from student where name = 'rec8'";
  auto p = planner.createQueryPlan(qry, transaction.get());
  auto s = p->open();
  while (s->next()) {
    std::cout << "id: " << s->getInt("id") << std::endl;
  }
  s->close();
  transaction->commit();
}
