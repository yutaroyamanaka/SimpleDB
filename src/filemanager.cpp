/* Copyright 2021 Yutaro Yamanaka */
#include "filemanager.hpp"
#include <fstream>
#include <filesystem>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

namespace file {
FileManager::FileManager(const std::filesystem::path db_directory, int block_size) : db_directory_(db_directory), block_size_(block_size) {
  std::filesystem::directory_entry dir(db_directory);

  is_new_ = !dir.exists();

  // create the directory if the database is new
  if(is_new_) {
    std::filesystem::create_directories(db_directory);
  }

  // remove any leftover temp tables
  for(auto& p : std::filesystem::directory_iterator(db_directory)) {
    if(p.path().filename().string().find("temp")) {
      std::filesystem::remove(p.path());
    }
  }
} 

void FileManager::write(BlockId& block_id, Page& page) {
  std::unique_lock<std::mutex> lock(mutex_);
  const std::string file_name = block_id.fileName();

  std::shared_ptr<std::fstream> fileIO = getFile(file_name);
  size_t offset = block_id.number() * block_size_;
  fileIO->seekp(offset, std::ios::beg);
  fileIO->write(&((*page.contents())[0]), block_size_);
  if(fileIO->bad()) {
    exit(1);
  }
  fileIO->flush();
}

void FileManager::read(BlockId& block_id, Page& page) {
  std::unique_lock<std::mutex> lock(mutex_);
  const std::string file_name = block_id.fileName();
  std::shared_ptr<std::fstream> fileIO = getFile(file_name);
  size_t offset = block_id.number() * block_size_;

  fileIO->seekp(offset, std::ios::beg);
  fileIO->read(&((*page.contents())[0]), block_size_);
  if(fileIO->bad()) {
    exit(1);
  }

  int readCount = fileIO->gcount();
  if(readCount < block_size_) {
    fileIO->clear();
    memset(&((*page.contents())[readCount]),  0, block_size_ - readCount);
  }
}

BlockId FileManager::append(const std::string& file_name) {
  std::unique_lock<std::mutex> lock(mutex_);
  std::shared_ptr<std::fstream> fileIO = getFile(file_name);

  int newBlockNum = length(file_name);
  BlockId block_id(file_name, newBlockNum);

  std::vector<char> byte_vector(block_size_);
  
  fileIO->seekp(newBlockNum * block_size_, std::ios::beg);
  fileIO->write(&byte_vector[0], block_size_);
  if(fileIO->bad()) {
    exit(1);
  }
  fileIO->flush();
  return block_id;
}

std::shared_ptr<std::fstream> FileManager::getFile(const std::string& file_name) {
  auto fileIO = std::make_shared<std::fstream>();
  std::filesystem::path path = db_directory_ / file_name;

  if(open_files_.find(path) != open_files_.end()) {
    fileIO = open_files_[path];
    if(fileIO->is_open()) {
      return fileIO;
    }
  }

  fileIO->open(path.string(), std::ios::binary | std::ios::in | std::ios::out);
  if(!fileIO->is_open()) {
    fileIO->clear();
    fileIO->open(path.string(), std::ios::binary | std::ios::trunc | std::ios::in | std::ios::out);
    fileIO->close();
    fileIO->open(path.string(), std::ios::binary | std::ios::in | std::ios::out);

    if(!fileIO->is_open()) {
      exit(1);
    }
  }

  open_files_[path] = fileIO;
  return fileIO;
}

int FileManager::length(const std::string& file_name) {
  std::filesystem::path path = db_directory_ / file_name;
  int size;
  try {
    size = static_cast<int>(std::filesystem::file_size(path));
  } catch (std::exception &e) {
    size = 0;
  }
  
  return size / block_size_;
}
}
