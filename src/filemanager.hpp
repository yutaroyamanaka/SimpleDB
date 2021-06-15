#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include "blockid.hpp"
#include "page.hpp"

namespace file {
class FileManager {
  public:
    FileManager(const std::filesystem::path db_directory, int block_size);
    void read(BlockId& block_id, Page& page);
    void write(BlockId& block_id, Page& page);
    BlockId append(const std::string& file_name);
    int length(const std::string& file_name);
    bool isNew() { return is_new_; }
    int blockSize() { return block_size_; }
  private:
    std::filesystem::path db_directory_;
    int block_size_;
    bool is_new_;
    std::map<std::filesystem::path, std::shared_ptr<std::fstream>> open_files_;
    std::mutex mutex_;
    std::shared_ptr<std::fstream> getFile(const std::string& file_name);
};
}
