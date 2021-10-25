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
#include "parse/parser.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "plan/planner.hpp"
#include "scan/scan.hpp"
#include "tx/transaction.hpp"

void createDB7(const std::string& dbname) {
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
     "( 9, 'lee', 10, 2021 )",
     "( 9, 'lxe', 10, 2009 )",
     "( 9, 'leu', 10, 2009 )",
     "( 9, 'lea', 10, 2008 )",
     "( 9, 'beb', 10, 2008 )",
     "( 9, 'lec', 10, 2011 )",
     "( 9, 'led', 10, 2002 )",
     "( 9, 'lee', 10, 2003 )",
     "( 9, 'aef', 10, 2011 )",
     "( 9, 'leg', 10, 2001 )",
     "( 9, 'leh', 10, 2023 )",
     "( 9, 'lei', 10, 2011 )",
     "( 9, 'lek', 10, 2003 )",
     "( 9, 'lel', 10, 2011 )",
     "( 9, 'lem', 10, 2003 )",
     "( 9, 'len', 10, 2011 )",
     "( 9, 'leo', 10, 2001 )",
     "( 9, 'lep', 10, 2003 )",
     "( 9, 'laq', 10, 2011 )",
     "( 9, 'ler', 10, 2001 )",
     "( 9, 'lea', 10, 2001 )",
     "( 9, 'leu', 10, 2011 )",
     "( 9, 'ljs', 10, 2001 )",
     "( 9, 'lee', 10, 2002 )",
     "( 9, 'bje', 10, 2011 )",
     "( 9, 'lje', 10, 2001 )",
    };

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

void sortStudentData(const std::string& file_name, int block_size, int buffer_size) {
  app::SimpleDB db(file_name, block_size, buffer_size);
  auto transaction = db.getNewTx();
  plan::Planner& planner = db.getPlanner();

  std::string qry = "select sname, gradyear from student order by gradyear";
  std::string t1 = "sname";
  std::string t2 = "gradyear";

  auto pln = planner.createQueryPlan(qry, transaction.get());
  auto scan = pln->open();
  std::cout << "| " << t1 << " | " << t2 << " |" << std::endl;
  while (scan->next()) {
    std::cout << "| " << scan->getString(t1) << " | " << scan->getInt(t2)  << " |" << std::endl;
  }
  scan->close();
  transaction->commit();
  std::cout << std::endl;
}

TEST(MultiBufferSortTest, Main) {
  std::string file_name = "multibuffersortTest";
  createDB7(file_name);

  std::cout << "block_size: " << 400 << " " << "buffer_size : " << 8 << std::endl;
  sortStudentData(file_name, 400, 8);
  std::cout << "block_size: " << 200 << " " << "buffer_size : " << 10 << std::endl;
  sortStudentData(file_name, 200, 10);
  std::cout << "block_size: " << 100 << " " << "buffer_size : " << 12 << std::endl;
  sortStudentData(file_name, 100, 12);
  std::cout << "block_size: " << 50 << " " << "buffer_size : " << 16 << std::endl;
  sortStudentData(file_name, 50, 16);


  std::cout << "block_size: " << 20 << " " << "buffer_size : " << 4 << std::endl;
  sortStudentData(file_name, 20, 4);
  std::cout << "block_size: " << 15 << " " << "buffer_size : " << 16 << std::endl;
  sortStudentData(file_name, 15, 16);
}
