/* Copyright 2021 Yutaro Yamanaka */
#include "interface/connection.hpp"

namespace interface {
  Connection::Connection(std::unique_ptr<app::SimpleDB> db) : db_(std::move(db)), transaction_(db_->getNewTx()), planner_(db_->getPlanner()) {
  }

  std::unique_ptr<Statement >Connection::createStatement() {
    try {
      auto ptr = std::make_unique<Statement>(this, planner_);
      return ptr;
    } catch (std::exception& e) {
      throw std::runtime_error("cannot create statement");
    }
  }

  void Connection::close() {
    try {
      commit();
    } catch (std::exception& e) {
      throw std::runtime_error("cannot close connection");
    }
  }

  void Connection::commit() {
    try {
      transaction_->commit();
      transaction_ = db_->getNewTx();
    } catch (std::exception& e) {
      throw std::runtime_error("cannot commit");
    }
  }

  void Connection::rollback() {
    try {
      transaction_->rollback();
      transaction_ = db_->getNewTx();
    } catch (std::exception& e) {
      throw std::runtime_error("cannot commit");
    }
  }

  tx::Transaction& Connection::getTransaction() {
    return *transaction_;
  }
}  // namespace interface
