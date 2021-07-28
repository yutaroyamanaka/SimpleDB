#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "blockid.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"
#include "transaction.hpp"
#include "recordpage.hpp"
#include "schema.hpp"
#include "layout.hpp"
#include "tablescan.hpp"
#include "tablemanager.hpp"

TEST(CatalogTest, Main) {
  std::string file_name = "catalogtest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  log::LogManager lm(&file_manager, log_file_name);
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

  while(ts.next()) {
    std::string tname = ts.getString("tblname");
    int size = ts.getInt("slotsize");
    std::cout << tname << " " << size << std::endl;
  }
  ts.close();

  std::cout << "All fields and their offsets:" << std::endl;
  layout = tm.getLayout("fldcat", &tx);
  ts = record::TableScan(&tx, "fldcat", layout);

  while(ts.next()) {
    std::string tname = ts.getString("tblname");
    std::string fname = ts.getString("fldname");
    int offset = ts.getInt("offset");
    std::cout << tname << " " << fname << " " << offset << std::endl;
  }
  ts.close();
  tx.commit();
}
