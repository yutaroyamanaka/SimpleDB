/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "logging/logmanager.hpp"
#include "buffer/buffer.hpp"
#include "buffer/buffermanager.hpp"

TEST(BufferManagerTest, Main) {
  std::string file_name = "buffermanagerTest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;
  int buffer_size = 3;

  file::FileManager file_manager(path, block_size);
  logging::LogManager log_manager(&file_manager, log_file_name);
  buffer::BufferManager buffer_manager(&file_manager, &log_manager, buffer_size);

  std::vector<buffer::Buffer*> buff(6);
  file::BlockId block_id0("testfile", 0);
  file::BlockId block_id1("testfile", 1);
  file::BlockId block_id2("testfile", 2);

  buff[0] = buffer_manager.pin(block_id0);
  buff[1] = buffer_manager.pin(block_id1);
  buff[2] = buffer_manager.pin(block_id2);

  buffer_manager.unpin(buff[1]);
  buff[1] = nullptr;

  block_id0 = file::BlockId("testfile", 0);
  block_id1 = file::BlockId("testfile", 1);
  buff[3] = buffer_manager.pin(block_id0);
  buff[4] = buffer_manager.pin(block_id1);

  EXPECT_EQ(buffer_manager.available(), 0);

  file::BlockId block_id3("testfile", 3);
  try {
    std::cout << "Attempting to pin block 3..." << std::endl;
    buff[5] = buffer_manager.pin(block_id3);
  } catch (std::exception& e) {
    std::cout << "Exception: No Available buffers" << std::endl;
  }

  buffer_manager.unpin(buff[2]);
  buff[2] = nullptr;
  buff[5] = buffer_manager.pin(block_id3);

  EXPECT_EQ(buff[0]->block().number(), 0);
  EXPECT_EQ(buff[3]->block().number(), 0);
  EXPECT_EQ(buff[4]->block().number(), 1);
  EXPECT_EQ(buff[5]->block().number(), 3);
}
