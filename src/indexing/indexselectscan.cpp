/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/indexselectscan.hpp"

namespace indexing {
  IndexSelectScan::IndexSelectScan(const std::shared_ptr<record::TableScan>& ts, const std::shared_ptr<Index>& idx, const scan::Constant& val):
    ts_(ts), idx_(idx), val_(val) {
      beforeFirst();
    }

  void IndexSelectScan::beforeFirst() {
    idx_->beforeFirst(val_);
  }

  bool IndexSelectScan::next() {
    bool ok = idx_->next();
    if (ok) {
      record::RID rid = idx_->getDataRid();
      ts_->moveToRid(rid);
    }
    return ok;
  }

  int IndexSelectScan::getInt(const std::string& fldname) {
    return ts_->getInt(fldname);
  }

  std::string IndexSelectScan::getString(const std::string& fldname) {
    return ts_->getString(fldname);
  }

  scan::Constant IndexSelectScan::getVal(const std::string& fldname) {
    return ts_->getVal(fldname);
  }

  bool IndexSelectScan::hasField(const std::string& fldname) {
    return ts_->hasField(fldname);
  }

  void IndexSelectScan::close() {
    idx_->close();
    ts_->close();
  }
}  // namespace indexing
