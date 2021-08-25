/* Copyright 2021 Yutaro Yamanaka */
#include "interface/driver.hpp"

namespace interface {
  std::unique_ptr<Connection> Driver::connect(const std::string& dbname) {
    auto db = std::make_unique<app::SimpleDB>(dbname);
    auto conn = std::make_unique<Connection>(std::move(db));
    return conn;
  }
}  // namespace interface
