/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"
#include "record/recordpage.hpp"
#include "record/schema.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "meta/tablemanager.hpp"

TEST(TableManagerTest, Main) {
  std::string file_name = "tablemanagerTest";
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

  record::Layout layout = tm.getLayout("MyTable", &tx);
  int size = layout.slotSize();
  record::Schema sch2 = layout.schema();

  std::cout << "MyTable has slot size " << size << std::endl;
  std::cout << "Its fields are:" << std::endl;
  for (const auto& fldname : sch2.fields()) {
    std::string type;
    if (sch2.type(fldname) == record::Schema::INTEGER) {
      type = "int";
    } else {
      int strlen = sch2.length(fldname);
      type = "varchar(" + std::to_string(strlen) + ")";
    }
    std::cout << fldname << ": " << type << std::endl;
  }
  tx.commit();
}
