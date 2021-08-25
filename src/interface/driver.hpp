/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "app/simpledb.hpp"
#include "interface/connection.hpp"

namespace interface {
class Driver {
 public:
   std::unique_ptr<Connection> connect(const std::string& dbname);
};
}  // namespace interface
