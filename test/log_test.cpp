/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include "blockid.hpp"
#include "page.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"

void printLogRecord(log::LogManager& log_manager, std::string msg) {
  //std::cout << msg << std::endl;
  log::LogIterator iter = log_manager.iterator();

  while(iter.hasNext()) {
    auto rec = std::make_shared<std::vector<char>>(iter.next());
    file::Page page(rec);
    std::string s = page.getString(0);
    int npos = file::Page::maxLength(s.size());
    int val = page.getInt(npos);
    //std::cout << "[" << s << ", " << val << "]" << std::endl;
  }
  //std::cout << std::endl;
}

void createRecords(log::LogManager& log_manager, int start, int end) {
  for(int i = start; i <= end; i++) {
    std::string s = "record" + std::to_string(i);

    int npos = file::Page::maxLength(s.size());
    auto rec = std::make_shared<std::vector<char>>(npos + sizeof(uint32_t), 0);
    file::Page page(rec);
    page.setString(0, s);
    page.setInt(npos, i+100);
    int lsn = log_manager.append(*rec);
  }
}

TEST(LogTest, Main) {
  std::string file_name = "logtest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);

  log::LogManager lm(&file_manager, log_file_name);

  createRecords(lm, 1, 35);
  EXPECT_EQ(20, lm.getLastSavedLSN());

  std::string msg = "The log file now has these records:";
  printLogRecord(lm, msg);

  createRecords(lm, 36, 70);
  lm.flush(65);
  EXPECT_EQ(70, lm.getLastSavedLSN());

  std::string msg2 = "The log file now has these records:";
  printLogRecord(lm, msg2);
}
