/* Copyright 2021 Yutaro Yamanaka */
#include "app/simpledb.hpp"

namespace app {
  int SimpleDB::block_size_ = 400;
  int SimpleDB::buffer_size_ = 8;
  std::string SimpleDB::log_file_name_ = "simpledb.log";

  SimpleDB::SimpleDB() {
  }

  SimpleDB::SimpleDB(const std::string& filename, int block_size, int buffer_size) {
    auto path = std::filesystem::current_path() / filename;
    file_manager_ = std::make_unique<file::FileManager>(path, block_size_);
    log_manager_ = std::make_unique<logging::LogManager>(file_manager_.get(), log_file_name_);
    buffer_manager_ = std::make_unique<buffer::BufferManager>(file_manager_.get(), log_manager_.get(), buffer_size);

    auto transaction = std::make_unique<tx::Transaction>(file_manager_.get(), log_manager_.get(), buffer_manager_.get());
    bool isNew = file_manager_->isNew();
    if (isNew) {
      std::cout << "creating new database" << std::endl;
    } else {
      std::cout << "recovering existing database" << std::endl;
      transaction->recover();
    }

    meta_data_manager_ = std::make_unique<meta::MetaDataManager>(isNew, transaction.get());
    auto qp = std::make_unique<plan::BasicQueryPlanner>(meta_data_manager_.get());
    auto up = std::make_unique<indexing::IndexUpdatePlanner>(meta_data_manager_.get());
    planner_ = std::make_unique<plan::Planner>(std::move(qp), std::move(up));
    transaction->commit();
  }

  SimpleDB::SimpleDB(const std::string& filename) : SimpleDB(filename, block_size_, buffer_size_) {
    /*
    auto transaction = std::make_unique<tx::Transaction>(file_manager_.get(), log_manager_.get(), buffer_manager_.get());
    bool isNew = file_manager_->isNew();
    if (isNew) {
      std::cout << "creating new database" << std::endl;
    } else {
      std::cout << "recovering existing database" << std::endl;
      transaction->recover();
    }

    meta_data_manager_ = std::make_unique<meta::MetaDataManager>(isNew, transaction.get());
    auto qp = std::make_unique<plan::BasicQueryPlanner>(meta_data_manager_.get());
    auto up = std::make_unique<indexing::IndexUpdatePlanner>(meta_data_manager_.get());
    planner_ = std::make_unique<plan::Planner>(std::move(qp), std::move(up));
    transaction->commit();*/
  }

  void SimpleDB::tinySetup(const std::string& filename, int block_size, int buffer_size) {
    auto path = std::filesystem::current_path() / filename;
    file_manager_ = std::make_unique<file::FileManager>(path, block_size_);
    log_manager_ = std::make_unique<logging::LogManager>(file_manager_.get(), log_file_name_);
    buffer_manager_ = std::make_unique<buffer::BufferManager>(file_manager_.get(), log_manager_.get(), buffer_size);
  }

  file::FileManager& SimpleDB::getFileManager() {
    return *file_manager_;
  }

  logging::LogManager& SimpleDB::getLogManager() {
    return *log_manager_;
  }

  buffer::BufferManager& SimpleDB::getBufferManager() {
    return *buffer_manager_;
  }

  meta::MetaDataManager& SimpleDB::getMetaDataManager() {
    return *meta_data_manager_;
  }

  plan::Planner& SimpleDB::getPlanner() {
    return *planner_;
  }

  std::unique_ptr<tx::Transaction> SimpleDB::getNewTx() {
    auto txptr = std::make_unique<tx::Transaction>(file_manager_.get(), log_manager_.get(), buffer_manager_.get());
    return txptr;
  }
}  // namespace app
