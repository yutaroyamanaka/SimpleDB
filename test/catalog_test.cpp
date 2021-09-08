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
#include "meta/tablemanager.hpp"

TEST(CatalogTest, Main) {
  std::string file_name = "catalogTest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  logging::LogManager lm(&file_manager, log_file_name);
  buffer::BufferManager bm(&file_manager, &lm, 8);

  tx::Transaction tx(&file_manager, &lm, &bm);
  meta::TableManager tm(true, &tx);
  record::Schema sch;
  sch.addIntField("A");
  sch.addStringField("B", 9);
  tm.createTable("MyTable", sch, &tx);

  std::cout << "All tables and their lengths:" << std::endl;
  record::Layout layout = tm.getLayout("tblcat", &tx);
  record::TableScan ts(&tx, "tblcat", layout);

  while (ts.next()) {
    std::string tname = ts.getString("tblname");
    int size = ts.getInt("slotsize");
    std::cout << tname << " " << size << std::endl;
  }
  ts.close();

  std::cout << "All fields and their offsets:" << std::endl;
  layout = tm.getLayout("fldcat", &tx);
  record::TableScan ts2(&tx, "fldcat", layout);

  while (ts2.next()) {
    std::string tname = ts2.getString("tblname");
    std::string fname = ts2.getString("fldname");
    int offset = ts2.getInt("offset");
    std::cout << tname << " " << fname << " " << offset << std::endl;
  }
  ts2.close();
  tx.commit();
}
