/* Copyright 2021 Yutaro Yamanaka */
#include <gtest/gtest.h>
#include <iostream>
#include "app/simpledb.hpp"
#include "file/blockid.hpp"
#include "file/page.hpp"
#include "file/filemanager.hpp"
#include "log/logmanager.hpp"

void printLogRecord(log::LogManager& log_manager, std::string& msg) {
  log::LogIterator iter = log_manager.iterator();

  while (iter.hasNext()) {
    auto rec = std::make_shared<std::vector<char>>(iter.next());
    file::Page page(rec);
    std::string s = page.getString(0);
    int npos = file::Page::maxLength(s.size());
    int val = page.getInt(npos);
  }
}

void createRecords(log::LogManager& log_manager, int start, int end) {
  for (int i = start; i <= end; i++) {
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
  std::string file_name = "logTest";
  app::SimpleDB db(file_name, 400, 8);

  log::LogManager &lm = db.getLogManager();

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
