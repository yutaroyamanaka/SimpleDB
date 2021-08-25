/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <algorithm>
#include <string>
#include "record/schema.hpp"

namespace interface {
class MetaData {
 public:
   MetaData(const record::Schema& sch);
   int getColumnCount();
   std::string getColumnName(int column);
   int getColumnType(int column);
   int getColumnDisplaySize(int column);
 private:
   record::Schema sch_;
};
}  // namespace interface
