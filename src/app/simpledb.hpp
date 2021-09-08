/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "file/filemanager.hpp"
#include "indexing/indexupdateplanner.hpp"
#include "logging/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"
#include "meta/metadatamanager.hpp"
#include "plan/planner.hpp"
#include "plan/basicqueryplanner.hpp"
//#include "plan/basicupdateplanner.hpp"

namespace app {
class SimpleDB {
 public:
    SimpleDB(const std::string& filename, int block_size, int buffr_size);
    SimpleDB(const std::string& filename);
    file::FileManager& getFileManager();
    buffer::BufferManager& getBufferManager();
    logging::LogManager& getLogManager();
    meta::MetaDataManager& getMetaDataManager();
    plan::Planner& getPlanner();
    std::unique_ptr<tx::Transaction> getNewTx();

    static int block_size_;
    static int buffer_size_;
    static std::string log_file_name_;
 private:
    std::unique_ptr<file::FileManager> file_manager_;
    std::unique_ptr<buffer::BufferManager> buffer_manager_;
    std::unique_ptr<logging::LogManager> log_manager_;
    std::unique_ptr<meta::MetaDataManager> meta_data_manager_;
    std::unique_ptr<plan::Planner> planner_;
};
}  // namespace app
