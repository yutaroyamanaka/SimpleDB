/* Copyright 2021 Yutaro Yamanaka */
#include "productscan.hpp"

namespace scan {
  ProductScan::ProductScan(std::shared_ptr<Scan>& s1, std::shared_ptr<Scan>& s2) : s1_(s1), s2_(s2) {
    s1_->next();
  }

  void ProductScan::beforeFirst() {
    s1_->beforeFirst();
    s1_->next();
    s2_->beforeFirst();
  }

  bool ProductScan::next() {
    if (s2_->next()) return true;
    s2_->beforeFirst();
    return s2_->next() && s1_->next();
  }

  int ProductScan::getInt(const std::string& fldname) {
    if (s1_->hasField(fldname)) return s1_->getInt(fldname);
    return s2_->getInt(fldname);
  }

  std::string ProductScan::getString(const std::string& fldname) {
    if (s1_->hasField(fldname)) return s1_->getString(fldname);
    return s2_->getString(fldname);
  }

  Constant ProductScan::getVal(const std::string& fldname) {
    if (s1_->hasField(fldname)) return s1_->getVal(fldname);
    return s2_->getVal(fldname);
  }

  bool ProductScan::hasField(const std::string& fldname) {
    return s1_->hasField(fldname) || s2_->hasField(fldname);
  }

  void ProductScan::close() {
    s1_->close();
    s2_->close();
  }
}  // namespace scan
