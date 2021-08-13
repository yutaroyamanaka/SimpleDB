/* Copyright 2021 Yutaro Yamanaka */
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

TEST(TableManagerTest, Main) {
  std::string file_name = "tablemanagertest";
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
