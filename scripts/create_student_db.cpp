/* Copyright 2021 Yutaro Yamanaka */
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "interface/connection.hpp"
#include "interface/driver.hpp"
#include "interface/statement.hpp"
#include "interface/resultset.hpp"

void createDB(const std::string& dbname) {
  try {
    auto driver = std::unique_ptr<interface::Driver>();
    auto conn = driver->connect(dbname);
    auto stmt = conn->createStatement();

    std::string s = "create table student ( sid int, sname varchar ( 10 ), majorid int, gradyear int )";
    stmt->executeUpdate(s);
    std::cout << "student table created" << std::endl;

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

void studentMajor(const std::string& dbname) {
  try {
    auto driver = std::unique_ptr<interface::Driver>();
    auto conn = driver->connect(dbname);
    auto stmt = conn->createStatement();

    std::string s = "select sname, dname from dept, student where majorid = did";
    std::cout << std::endl << s << std::endl;
    interface::ResultSet rs = stmt->executeQuery(s);
    std::string t1 = "sname";
    std::string t2 = "dname";

    std::cout << std::endl;
    std::cout << "| " << t1 << " | " << t2 << " |" << std::endl;
    while (rs.next()) {
      std::string sname = rs.getString(t1);
      std::string dname = rs.getString(t2);
      std::cout << "| " << sname << " | " << dname  << " |" << std::endl;
    }
    rs.close();
    std::cout << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

void findMajors(const std::string& dbname, const std::string& major) {
  try {
    auto driver = std::unique_ptr<interface::Driver>();
    auto conn = driver->connect(dbname);
    auto stmt = conn->createStatement();

    std::string s = "select sname, gradyear from student, dept where majorid = did and dname = '" + major + "'";
    std::cout << std::endl << s << std::endl;
    interface::ResultSet rs = stmt->executeQuery(s);
    std::string t1 = "sname";
    std::string t2 = "gradyear";

    std::cout << std::endl;
    std::cout << "| " << t1 << " | " << t2 << " |" << std::endl;
    while (rs.next()) {
      std::string sname = rs.getString(t1);
      int gradyear = rs.getInt(t2);
      std::cout << "| " << sname << " | " << gradyear  << " |" << std::endl;
    }
    std::cout << std::endl;
    rs.close();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

int main(int argc, char** argv) {
  std::string dbname = "student";
  createDB(dbname);
  std::cout << std::endl;

  std::cout << "Enter a department name: ";
  std::string major;
  std::cin >> major;

  studentMajor(dbname);
  findMajors(dbname, major);
  return 0;
}
