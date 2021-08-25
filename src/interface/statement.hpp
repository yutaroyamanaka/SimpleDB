/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "interface/connection.hpp"
#include "interface/resultset.hpp"
#include "plan/planner.hpp"

namespace interface {
class Connection;

class ResultSet;

class Statement {
 public:
    Statement(Connection* conn, plan::Planner& planner);
    ResultSet executeQuery(const std::string& qry);
    int executeUpdate(const std::string& cmd);
    void close();
 private:
    Connection* conn_;
    plan::Planner& planner_;
};
}  // interface
