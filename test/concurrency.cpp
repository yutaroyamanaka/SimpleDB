#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "blockid.hpp"
#include "page.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"
#include "transaction.hpp"

void runA(file::FileManager& fm, log::LogManager& lm, buffer::BufferManager& bm) {
  try {
    tx::Transaction txA(&fm, &lm, &bm);
    file::BlockId blk1("testfile", 1);
    file::BlockId blk2("testfile", 2);
    txA.pin(blk1);
    txA.pin(blk2);
    std::cout << "Tx A: request slock 1" << std::endl;
    txA.getInt(blk1, 0);
    std::cout << "Tx A: receive slock 1" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Tx A: request slock 2" << std::endl;
    txA.getInt(blk2, 0);
    std::cout << "Tx A: receive slock 2" << std::endl;
    txA.commit();
  } catch(std::exception& e) {
    std::cout << "Transaction A";
    std::cout << e.what() << std::endl;
  }
}

void runB(file::FileManager& fm, log::LogManager& lm, buffer::BufferManager& bm) {
  try {
    tx::Transaction txB(&fm, &lm, &bm);
    file::BlockId blk1("testfile", 1);
    file::BlockId blk2("testfile", 2);
    txB.pin(blk1);
    txB.pin(blk2);
    std::cout << "Tx B: request xlock 2" << std::endl;
    txB.setInt(blk2, 0, 0, false);
    std::cout << "Tx B: receive xlock 2" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Tx B: request slock 1" << std::endl;
    txB.getInt(blk1, 0);
    std::cout << "Tx B: receive slock 1" << std::endl;
    txB.commit();
  } catch(std::exception& e) {
    std::cout << "Transaction B";
    std::cout << e.what() << std::endl;
  }
}

void runC(file::FileManager& fm, log::LogManager& lm, buffer::BufferManager& bm) {
  try {
    tx::Transaction txC(&fm, &lm, &bm);
    file::BlockId blk1("testfile", 1);
    file::BlockId blk2("testfile", 2);
    txC.pin(blk1);
    txC.pin(blk2);
    std::cout << "Tx C: request xlock 1" << std::endl;
    txC.setInt(blk1, 0, 0, false);
    std::cout << "Tx C: receive xlock 1" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Tx C: request slock 2" << std::endl;
    txC.getInt(blk1, 0);
    std::cout << "Tx C: receive slock 2" << std::endl;
    txC.commit();

  } catch(std::exception& e) {
    std::cout << "Transaction C";
    std::cout << e.what() << std::endl;
  }
}

TEST(ConcurrencyTest, Main) {
  std::string file_name = "concurrencytest";
  std::string log_file_name = "simpledb.log";

  int block_size = 400;
  auto path = std::filesystem::current_path() / file_name;

  file::FileManager file_manager(path, block_size);
  log::LogManager lm(&file_manager, log_file_name);
  buffer::BufferManager bm(&file_manager, &lm, 8);

  std::thread A(runA, std::ref(file_manager), std::ref(lm), std::ref(bm));
  std::thread B(runB, std::ref(file_manager), std::ref(lm), std::ref(bm));
  std::thread C(runC, std::ref(file_manager), std::ref(lm), std::ref(bm));

  A.join();
  B.join();
  C.join();
}
