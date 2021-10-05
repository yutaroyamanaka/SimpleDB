/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "app/simpledb.hpp"
#include "interface/connection.hpp"
#include "interface/driver.hpp"
#include "interface/statement.hpp"
#include "interface/resultset.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "plan/planner.hpp"
#include "scan/scan.hpp"
#include "tx/transaction.hpp"

void createDB2(const std::string& dbname) {
  try {
    auto driver = std::unique_ptr<interface::Driver>();
    auto conn = driver->connect(dbname);
    auto stmt = conn->createStatement();

    std::string s = "create table student ( sid int, sname varchar ( 10 ), majorid int, gradyear int )";
    stmt->executeUpdate(s);
    std::cout << "student table created" << std::endl;

    s = "create index majorid on student ( majorid )";
    stmt->executeUpdate(s);
    std::cout << s << " created" << std::endl;

    s = "insert into student ( sid, sname, majorid, gradyear ) values ";
    std::vector<std::string> studvals =
    {"( 1, 'joe', 10, 2021 )",
     "( 2, 'amy', 20, 2020 )",
     "( 3, 'max', 10, 2022 )",
     "( 4, 'sue', 20, 2022 )",
     "( 5, 'bob', 30, 2020 )",
     "( 6, 'kim', 20, 2020 )",
     "( 7, 'art', 30, 2021 )",
     "( 8, 'pat', 20, 2019 )",
     "( 9, 'lee', 10, 2021 )"};

    for (const auto& val : studvals) {
      stmt->executeUpdate(s + val);
    }
    std::cout << "student data inserted" << std::endl;

    s = "create table dept ( did int, dname varchar ( 8 ) )";
    stmt->executeUpdate(s);
    std::cout << "dept table created" << std::endl;

    s = "insert into dept ( did, dname ) values ";
    std::vector<std::string> deptvals =
    {"( 10, 'compsci' )",
     "( 20, 'math' )",
     "( 30, 'drama' )"};

    for (const auto& val : deptvals) {
      stmt->executeUpdate(s + val);
    }
    std::cout << "dept data inserted" << std::endl;

    s = "create table section ( sectid int, courseid int, prof varchar ( 8 ), yearoffered int )";
    stmt->executeUpdate(s);
    std::cout << "section table created" << std::endl;

    s = "insert into section ( sectid, courseid, prof, yearoffered ) values ";
    std::vector<std::string> sectvals =
    {"( 13, 12, 'turing', 2018 )",
     "( 23, 12, 'turing', 2019 )",
     "( 33, 32, 'newton', 2019 )",
     "( 43, 32, 'einstein', 2017 )",
     "( 53, 62, 'brando', 2018 )"};

    for (const auto& val : sectvals) {
      stmt->executeUpdate(s + val);
    }
    std::cout << "sect data inserted" << std::endl;

    s = "create table enroll ( eId int, studentid int, sectionid int, grade varchar ( 2 ) )";
    stmt->executeUpdate(s);
    std::cout << "enroll table created" << std::endl;

    s = "insert into enroll ( eid, studentid, sectionid, grade ) values ";
    std::vector<std::string> enrollvals =
    {"( 14, 1, 13, 'A' )",
     "( 24, 1, 43, 'C' )",
     "( 34, 2, 43, 'B+' )",
     "( 44, 4, 33, 'B' )",
     "( 54, 4, 53, 'A' )",
     "( 64, 6, 53, 'A' )"};

    for (const auto& val : enrollvals) {
      stmt->executeUpdate(s + val);
    }
    std::cout << "enroll data inserted" << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

TEST(IndexUpdateTest, Main) {
  std::string file_name = "indexreupdateTest";
  createDB2(file_name);

  app::SimpleDB db(file_name);
  auto transaction = db.getNewTx();
  auto mdm = db.getMetaDataManager();

  // open an scan on the data table
  auto studentplan = std::static_pointer_cast<plan::Plan>(std::make_shared<plan::TablePlan>(transaction.get(), "student", &mdm));
  auto studentscan = std::static_pointer_cast<scan::UpdateScan>(studentplan->open());

  // create a map containing all indexes for student
  std::map<std::string, std::shared_ptr<indexing::Index>> indexes;
  std::map<std::string, meta::IndexInfo> idxinfo = mdm.getIndexInfo("student", transaction.get());
  for (const auto& [fldname, ii] : idxinfo) {
    indexes[fldname] = idxinfo[fldname].open();
  }

  // Task 1: Insert a new STUDENT recorrd for sam
  studentscan->insert();
  studentscan->setInt("sid", 11);
  studentscan->setString("sname", "'sam'");
  studentscan->setInt("gradyear", 2023);
  studentscan->setInt("majorid", 30);
  // Then insert a record into each of the indexes
  record::RID datarid = studentscan->getRid();
  for (const auto& [fldname, ii] : indexes) {
    scan::Constant dataval = studentscan->getVal(fldname);
    auto idx = indexes[fldname];
    idx->insert(dataval, datarid);
  }

  // Task 2: Find and delete joe's record
  studentscan->beforeFirst();
  while (studentscan->next()) {
    std::cout << "sname: " <<studentscan->getString("sname") << std::endl;
    if (studentscan->getString("sname") == "'joe'") {
      // First, delete the index records for Joe.
      record::RID joeRid = studentscan->getRid();
      for (const auto& [fldname, ii] : indexes) {
        scan::Constant dataval = studentscan->getVal(fldname);
        auto idx = indexes[fldname];
        idx->remove(dataval, datarid);
      }
      // Then delete Joe's record in STUDENT
      studentscan->remove();
      break;
    }
  }

  // Print the records to verify the updates
  studentscan->beforeFirst();
  while (studentscan->next()) {
    std::cout << "sname: " << studentscan->getString("sname") << " sid: " << studentscan->getInt("sid") << std::endl;
  }
  studentscan->close();
  for (const auto& [_, idx] : indexes) {
    idx->close();
  }
  transaction->commit();
}
