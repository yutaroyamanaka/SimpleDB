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

TEST(RecordTest, Main) {
  std::string file_name = "recordTest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  log::LogManager lm(&file_manager, log_file_name);
  buffer::BufferManager bm(&file_manager, &lm, 8);

  tx::Transaction tx1(&file_manager, &lm, &bm);
  record::Schema sch;
  sch.addIntField("A");
  sch.addStringField("B", 9);
  record::Layout layout(sch);

  for (auto& fldname : layout.schema().fields()) {
    int offset = layout.offset(fldname);
    std::cout << fldname + " has offset " + std::to_string(offset) << std::endl;
  }

  file::BlockId blk = tx1.append("recordfile");
  tx1.pin(blk);
  record::RecordPage rp(&tx1, blk, layout);
  rp.format();
  std::cout << "Filling the page with random records." << std::endl;
  int slot = rp.insertAfter(-1);

  int i = 0;
  while (slot >= 0) {
    int n = 23 + i;
    rp.setInt(slot, "A", n);
    rp.setString(slot, "B", "rec" + std::to_string(n));
    std::cout << "inserting into slot " << slot << ": {" << n << ", rec" << n << "}" << std::endl;
    slot = rp.insertAfter(slot);
    i++;
  }

  std::cout << "Deleted these records with A-values < 25." << std::endl;
  int count = 0;
  slot = rp.nextAfter(-1);
  while (slot >= 0) {
    int a = rp.getInt(slot, "A");
    std::string b = rp.getString(slot, "B");
    if (a < 25) {
      count++;
      std::cout << "slot " << slot << ": {" << a << ", " << b << "}" << std::endl;
      rp.remove(slot);
    }
    slot = rp.nextAfter(slot);
  }

  std::cout << count << " values under 25 were deleted" << std::endl;
  std::cout << "Here are the remaining records." << std::endl;
  slot = rp.nextAfter(-1);

  while (slot >= 0) {
    int a = rp.getInt(slot, "A");
    std::string b = rp.getString(slot, "B");
    std::cout << "slot " << slot << ": {" << a << ", " << b << "}" << std::endl;
    slot = rp.nextAfter(slot);
  }

  tx1.unpin(blk);
  tx1.commit();
}
