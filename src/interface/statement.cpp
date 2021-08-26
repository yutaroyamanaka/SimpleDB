/* Copyright 2021 Yutaro Yamanaka */
#include "interface/statement.hpp"

namespace interface {
  Statement::Statement(Connection* conn, plan::Planner& planner) : conn_(conn), planner_(planner) {
  }

  ResultSet Statement::executeQuery(const std::string& qry) {
    try {
      tx::Transaction& transaction = conn_->getTransaction();
      auto pln = planner_.createQueryPlan(qry, &transaction);
      ResultSet rs(pln.get(), conn_);
      return rs;
    } catch (std::exception& e) {
      conn_->rollback();
      std::runtime_error("cannot execute Query");
    }
  }

  int Statement::executeUpdate(const std::string& cmd) {
    try {
      tx::Transaction& transaction = conn_->getTransaction();
      int result = planner_.executeUpdate(cmd, &transaction);
      conn_->commit();
      return result;
    } catch (std::exception& e) {
      conn_->rollback();
      std::runtime_error("cannot execute update");
      return 0;
    }
  }

  void Statement::close() {
    return;
  }
}  // namespace interface
