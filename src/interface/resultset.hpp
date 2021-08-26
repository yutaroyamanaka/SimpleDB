/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <memory>
#include <string>
#include "interface/connection.hpp"
#include "interface/metadata.hpp"
#include "plan/plan.hpp"
#include "scan/scan.hpp"
#include "record/schema.hpp"

namespace interface {
class Connection;

class ResultSet {
 public:
   ResultSet(plan::Plan* p, Connection* conn);
   bool next();
   int getInt(std::string& fldname) const;
   std::string getString(std::string& fldname) const;
   MetaData getMetaData() const;
   void close();
 private:
   std::shared_ptr<scan::Scan> s_;
   record::Schema sch_;
   Connection* conn_;
};
}  // namespace interface
