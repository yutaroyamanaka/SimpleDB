/* Copyright 2021 Yutaro Yamanaka */
#include "projectscan.hpp"

namespace scan {
  ProjectScan::ProjectScan(std::shared_ptr<Scan>& s, const std::vector<std::string>& field_list) : s_(s), field_list_(field_list) {
  }

  void ProjectScan::beforeFirst() {
    s_->beforeFirst();
  }

  bool ProjectScan::next() {
    return s_->next();
  }

  int ProjectScan::getInt(const std::string& fldname) {
    if (hasField(fldname)) return s_->getInt(fldname);
    throw std::runtime_error("field not found");
  }

  std::string ProjectScan::getString(const std::string& fldname) {
    if (hasField(fldname)) return s_->getString(fldname);
    throw std::runtime_error("field not found");
  }

  Constant ProjectScan::getVal(const std::string& fldname) {
    if (hasField(fldname)) return s_->getVal(fldname);
    throw std::runtime_error("field not found");
  }

  bool ProjectScan::hasField(const std::string& fldname) {
    auto itr = std::find(field_list_.begin(), field_list_.end(), fldname);
    if (itr == field_list_.end()) false;
    return true;
  }

  void ProjectScan::close() {
    s_->close();
  }
}  // namespace scan
