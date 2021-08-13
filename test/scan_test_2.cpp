/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include "tx/transaction.hpp"
#include "record/schema.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "app/simpledb.hpp"
#include "scan/projectscan.hpp"
#include "scan/productscan.hpp"
#include "scan/selectscan.hpp"

TEST(ScanTest2, Main) {
  std::string file_name = "scantest2";
  app::SimpleDB db(file_name, 400, 8);
  auto transaction = db.getNewTx();

  record::Schema sch1;
  sch1.addIntField("A");
  sch1.addStringField("B", 9);
  record::Layout layout1(sch1);
  auto s1 = std::static_pointer_cast<scan::UpdateScan>(
      std::make_shared<record::TableScan>(transaction.get(), "T1", layout1));
  s1->beforeFirst();

  for (int i = 0; i < 100; i++) {
    s1->insert();
    int n = i;
    s1->setInt("A", n);
    s1->setString("B", "rec" + std::to_string(n));
  }
  s1->close();

  record::Schema sch2;
  sch2.addIntField("C");
  sch2.addStringField("D", 9);
  record::Layout layout2(sch2);
  auto s2 = std::static_pointer_cast<scan::UpdateScan>(
      std::make_shared<record::TableScan>(transaction.get(), "T2", layout2));
  s2->beforeFirst();

  for (int i = 0; i < 100; i++) {
    s2->insert();
    int n = i;
    s2->setInt("C", 100 - n - 1);
    s2->setString("D", "rec" + std::to_string(100 - n - 1));
  }
  s2->close();

  auto s3 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<record::TableScan>(transaction.get(), "T1", layout1));
  auto s4 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<record::TableScan>(transaction.get(), "T2", layout2));
  auto s5 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<scan::ProductScan>(s3, s4));

  scan::Term t(scan::Expression("A"), scan::Expression("C"));
  scan::Predicate pred(t);
  std::cout <<"The predicate is " << pred.toString() << std::endl;

  auto s6 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<scan::SelectScan>(s5, pred));

  std::vector<std::string> c = {"B", "D"};
  auto s7 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<scan::ProjectScan>(s6, c));
  while (s7->next()) {
    std::cout << s7->getString("B") << " " << s7->getString("D") << std::endl;
  }
  s7->close();
  transaction->commit();
}
