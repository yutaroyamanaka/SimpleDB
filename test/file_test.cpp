/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"

TEST(FileTest, Main) {
  std::string file_name = "fileTest";
  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  file::BlockId block_id("testfile", 2);
  auto page1 = std::make_shared<file::Page>(file_manager.blockSize());
  int pos1 = 88;

  std::string content = "abcdefghijklm";
  page1->setString(pos1, content);

  int size = file::Page::maxLength(content.length());
  int pos2 = pos1 + size;
  uint32_t n = 345;
  page1->setInt(pos2, n);
  file_manager.write(block_id, *page1);

  auto page2 = std::make_shared<file::Page>(file_manager.blockSize());
  file_manager.read(block_id, *page2);

  EXPECT_EQ(345, page2->getInt(pos2));
  EXPECT_EQ("abcdefghijklm", page2->getString(pos1));
}
