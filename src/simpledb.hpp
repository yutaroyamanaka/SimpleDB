/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"
#include "transaction.hpp"
#include "metadatamanager.hpp"

namespace simpledb {
class SimpleDB {
 public:
    SimpleDB(const std::string& filename, int block_size, int buffr_size);
    SimpleDB(const std::string& filename);
    file::FileManager& getFileManager();
    buffer::BufferManager& getBufferManager();
    log::LogManager& getLogManager();
    meta::MetaDataManager& getMetaDataManager();
    std::unique_ptr<tx::Transaction> getNewTx();

    static int block_size_;
    static int buffer_size_;
    static std::string log_file_name_;
 private:
    std::unique_ptr<file::FileManager> file_manager_;
    std::unique_ptr<buffer::BufferManager> buffer_manager_;
    std::unique_ptr<log::LogManager> log_manager_;
    std::unique_ptr<meta::MetaDataManager> meta_data_manager_;
};
}  // namespace simpledb
