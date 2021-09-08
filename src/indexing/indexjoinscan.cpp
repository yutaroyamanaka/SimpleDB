/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/indexjoinscan.hpp"

namespace indexing {
  IndexJoinScan::IndexJoinScan(const std::shared_ptr<scan::Scan>& lhs, const std::shared_ptr<Index>& idx,
      const std::string& joinfld, const std::shared_ptr<record::TableScan>& rhs) : lhs_(lhs), idx_(idx), joinfld_(joinfld), rhs_(rhs) {
    beforeFirst();
  }

  void IndexJoinScan::beforeFirst() {
    lhs_->beforeFirst();
    lhs_->next();
    resetIndex();
  }

  bool IndexJoinScan::next() {
    while (true) {
      if (idx_->next()) {
        rhs_->moveToRid(idx_->getDataRid());
        return true;
      }
      if (!lhs_->next()) {
        return false;
      }
      resetIndex();
    }
  }

  int IndexJoinScan::getInt(const std::string& fldname) {
    if (rhs_->hasField(fldname)) return rhs_->getInt(fldname);
    return lhs_->getInt(fldname);
  }

  std::string IndexJoinScan::getString(const std::string& fldname) {
    if (rhs_->hasField(fldname)) return rhs_->getString(fldname);
    return lhs_->getString(fldname);
  }

  scan::Constant IndexJoinScan::getVal(const std::string& fldname) {
    if (rhs_->hasField(fldname)) return rhs_->getVal(fldname);
    return lhs_->getVal(fldname);
  }

  bool IndexJoinScan::hasField(const std::string& fldname) {
    return rhs_->hasField(fldname) || lhs_->hasField(fldname);
  }

  void IndexJoinScan::close() {
    lhs_->close();
    idx_->close();
    rhs_->close();
  }

  void IndexJoinScan::resetIndex() {
    scan::Constant searchkey = lhs_->getVal(joinfld_);
    idx_->beforeFirst(searchkey);
  }
}  // namespace indexing
