/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"

TEST(RecoveryTest, Main) {
  std::string file_name = "recoverytest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  log::LogManager lm(&file_manager, log_file_name);
  buffer::BufferManager bm(&file_manager, &lm, 8);

  file::BlockId block_id0("testfile-recover", 0);
  file::BlockId block_id1("testfile-recover", 1);

  tx::Transaction tx1(&file_manager, &lm, &bm);
  tx::Transaction tx2(&file_manager, &lm, &bm);
  tx1.pin(block_id0);
  tx2.pin(block_id1);
  int pos = 0;
  for (int i = 0; i < 6; i++) {
      tx1.setInt(block_id0, pos, pos, true);
      tx2.setInt(block_id1, pos, pos, true);
      pos += static_cast<int>(sizeof(uint32_t));
  }
  tx1.setString(block_id0, 30, "abc", true);
  tx2.setString(block_id1, 30, "def", true);
  tx1.commit();
  tx2.commit();

  tx::Transaction tx3(&file_manager, &lm, &bm);
  tx::Transaction tx4(&file_manager, &lm, &bm);
  tx3.pin(block_id0);
  tx4.pin(block_id1);

  pos = 0;
  for (int i = 0; i < 6; i++) {
    int v1 = tx3.getInt(block_id0, pos);
    int v2 = tx4.getInt(block_id1, pos);
    EXPECT_EQ(pos, v1);
    EXPECT_EQ(pos, v2);
    tx3.setInt(block_id0, pos, pos + 100, true);
    tx4.setInt(block_id1, pos, pos + 100, true);
    pos += static_cast<int>(sizeof(uint32_t));
  }
  EXPECT_EQ("abc", tx3.getString(block_id0, 30));
  EXPECT_EQ("def", tx4.getString(block_id1, 30));

  tx3.setString(block_id0, 30, "uvw", true);
  tx4.setString(block_id1, 30, "xyz", true);

  bm.flushAll(tx3.getTransactionNum());
  bm.flushAll(tx4.getTransactionNum());
  tx3.rollback();
  tx4.forceCMClear();

  tx::Transaction tx5(&file_manager, &lm, &bm);
  tx5.recover();

  tx::Transaction tx6(&file_manager, &lm, &bm);
  tx6.pin(block_id1);
  pos = 0;

  for (int i = 0; i < 6; i++) {
    int v1 = tx6.getInt(block_id1, pos);
    EXPECT_EQ(pos, v1);
    pos += static_cast<int>(sizeof(uint32_t));
  }
  EXPECT_EQ("def", tx6.getString(block_id1, 30));
  tx6.commit();
}
