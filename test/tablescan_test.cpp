/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "logging/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"
#include "record/recordpage.hpp"
#include "record/schema.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"

TEST(TableScanTest, Main) {
  std::string file_name = "tablescanTest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  logging::LogManager lm(&file_manager, log_file_name);
  buffer::BufferManager bm(&file_manager, &lm, 8);

  tx::Transaction tx1(&file_manager, &lm, &bm);
  record::Schema sch;
  sch.addIntField("A");
  sch.addStringField("B", 9);
  record::Layout layout(sch);

  for (auto& fldname : layout.schema().fields()) {
    int offset = layout.offset(fldname);
    std::cout << fldname << " has offset " << std::to_string(offset) << std::endl;
  }

  record::TableScan ts(&tx1, "T", layout);
  std::cout << "Filling the table with 50 random records" << std::endl;
  ts.beforeFirst();
  int i = 0;

  for (int i = 0; i < 50; i++) {
    ts.insert();
    int n = i;
    ts.setInt("A", n);
    ts.setString("B", "rec" + std::to_string(n));
    std::cout << "inserting into slot " << ts.getRid().toString() << ": {" << std::to_string(n) << ", rec" << std::to_string(n) << "}" << std::endl;
    i++;
  }

  std::cout << "Deleted these records with A-values < 25." << std::endl;
  int count = 0;
  ts.beforeFirst();

  while (ts.next()) {
    int a = ts.getInt("A");
    std::string b = ts.getString("B");
    if (a < 25) {
      count++;
      std::cout << "slot " << ts.getRid().toString() << ": {" << a << ", " << b << "}" << std::endl;
      ts.remove();
    }
  }

  std::cout << count << " values under 25 were deleted" << std::endl;
  std::cout << "Here are the remaining records." << std::endl;
  ts.beforeFirst();

  while (ts.next()) {
    int a = ts.getInt("A");
    std::string b = ts.getString("B");
    std::cout << "slot " << ts.getRid().toString() << ": {" << a << ", " << b << "}" << std::endl;
  }

  ts.close();
  tx1.commit();
}
