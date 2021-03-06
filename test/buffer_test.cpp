/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "logging/logmanager.hpp"
#include "buffer/buffer.hpp"
#include "buffer/buffermanager.hpp"

TEST(BufferTest, Main) {
  std::string file_name = "bufferTest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;
  int buffer_size = 3;

  file::FileManager file_manager(path, block_size);
  logging::LogManager log_manager(&file_manager, log_file_name);
  buffer::BufferManager buffer_manager(&file_manager, &log_manager, buffer_size);

  file::BlockId block_id1("testfile", 1);
  buffer::Buffer* buff1 = buffer_manager.pin(block_id1);

  auto page = buff1->contents();
  int n = page->getInt(80);
  page->setInt(80, n+1);  // this modification will get written to disk.
  buff1->setModified(1, 0);

  buffer_manager.unpin(buff1);

  file::BlockId block_id2("testfile", 2);
  file::BlockId block_id3("testfile", 3);
  file::BlockId block_id4("testfile", 4);
  // one of these pins will flush buff1 to disk
  buffer::Buffer* buff2 = buffer_manager.pin(block_id2);
  buffer::Buffer* buff3 = buffer_manager.pin(block_id3);
  buffer::Buffer* buff4 = buffer_manager.pin(block_id4);

  buffer_manager.unpin(buff2);
  block_id1 = file::BlockId("testfile", 1);
  buff2 = buffer_manager.pin(block_id1);
  auto page2 = buff2->contents();
  page2->setInt(80, 9999);
  buff2->setModified(1, 0);
  buffer_manager.unpin(buff2);
}
