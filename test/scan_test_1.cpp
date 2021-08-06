#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include "transaction.hpp"
#include "schema.hpp"
#include "layout.hpp"
#include "tablescan.hpp"
#include "simpledb.hpp"
#include "projectscan.hpp"
#include "selectscan.hpp"

TEST(ScanTest1, Main) {
  std::string file_name = "scantest1";
  simpledb::SimpleDB db(file_name, 400, 8);
  auto transaction = db.getNewTx();

  record::Schema sch;
  sch.addIntField("A");
  sch.addStringField("B", 9);
  record::Layout layout(sch);
  auto s1 = std::static_pointer_cast<scan::UpdateScan>(
      std::make_shared<record::TableScan>(transaction.get(), "T", layout)
      );
  s1->beforeFirst();

  for(int i = 0; i < 100; i++) {
    s1->insert();
    int n = i;
    s1->setInt("A", n);
    s1->setString("B", "rec" + std::to_string(n));
  }
  s1->close();
  auto s2 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<record::TableScan>(transaction.get(), "T", layout)
      );

  int target = 10;
  scan::Constant c(target);
  scan::Term t(scan::Expression("A"), scan::Expression(c));
  scan::Predicate pred(t);

  std::cout <<"The predicate is " << pred.toString() << std::endl;
  auto s3 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<scan::SelectScan>(s2, pred)
      );
  std::vector<std::string> fields = {"B"};
  auto s4 = std::static_pointer_cast<scan::Scan>(
      std::make_shared<scan::ProjectScan>(s3, fields)
      );
  while(s4->next()) {
    std::cout << s4->getString("B") << std::endl;
  }
  s4->close();
  transaction->commit();
}
