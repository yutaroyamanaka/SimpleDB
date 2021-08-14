/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"

TEST(TxTest, Main) {
  std::string file_name = "txtest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  log::LogManager lm(&file_manager, log_file_name);
  buffer::BufferManager bm(&file_manager, &lm, 8);

  tx::Transaction tx1(&file_manager, &lm, &bm);
  file::BlockId blk("testfile", 1);

  tx1.pin(blk);
  tx1.setInt(blk, 80, 1, false);
  tx1.setString(blk, 40, "one", false);
  tx1.commit();

  tx::Transaction tx2(&file_manager, &lm, &bm);
  tx2.pin(blk);
  int ival = tx2.getInt(blk, 80);
  std::string sval = tx2.getString(blk, 40);
  EXPECT_EQ(1, ival);
  EXPECT_EQ("one", sval);

  int newival = ival + 1;
  std::string newsval = sval + "!";
  tx2.setInt(blk, 80, newival, true);
  tx2.setString(blk, 40, newsval, true);
  tx2.commit();

  tx::Transaction tx3(&file_manager, &lm, &bm);
  tx3.pin(blk);

  ival = tx3.getInt(blk, 80);
  sval = tx3.getString(blk, 40);
  EXPECT_EQ(2, ival);
  EXPECT_EQ("one!", sval);

  tx3.setInt(blk, 80, 9999, true);
  ival = tx3.getInt(blk, 80);
  EXPECT_EQ(9999, ival);
  tx3.rollback();

  tx::Transaction tx4(&file_manager, &lm, &bm);
  tx4.pin(blk);
  ival = tx4.getInt(blk, 80);
  EXPECT_EQ(2, ival);
  tx4.commit();
}
