#include <blockid.hpp>
#include <page.hpp>
#include <filemanager.hpp>
#include <filesystem>
#include <iostream>

int main(int argc, char** argv) {
  std::string file_name = "filetest";
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

  std::cout << "offset " << pos2 << std::endl;
  std::cout << " contains " << page2->getInt(pos2) << std::endl; // shoule be 345
  std::cout << "offset " << pos1 << std::endl; 
  std::cout << " contains " << page2->getString(pos1) << std::endl;  // shoule be abcdefghijklm
  return 0;
}
