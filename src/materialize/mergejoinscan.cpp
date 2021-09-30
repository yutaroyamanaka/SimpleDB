/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/mergejoinscan.hpp"

namespace materialize {
  MergeJoinScan::MergeJoinScan(const std::shared_ptr<scan::Scan>& s1, const std::shared_ptr<SortScan>& s2, const std::string& fldname1, const std::string& fldname2) :
    s1_(s1), s2_(s2), fldname1_(fldname1), fldname2_(fldname2) {
      beforeFirst();
    }

  void MergeJoinScan::close() {
    s1_->close();
    s2_->close();
  }

  void MergeJoinScan::beforeFirst() {
    s1_->beforeFirst();
    s2_->beforeFirst();
  }

  bool MergeJoinScan::next() {
    bool hasmore2 = s2_->next();
    if (hasmore2 && s2_->getVal(fldname2_) == joinval_) return true;
    bool hasmore1 = s1_->next();
    if (hasmore1 && s1_->getVal(fldname1_) == joinval_) {
      s2_->restorePosition();
      return true;
    }

    while (hasmore1 && hasmore2) { 
      scan::Constant v1 = s1_->getVal(fldname1_);
      scan::Constant v2 = s2_->getVal(fldname2_);
      if (v1 < v2) hasmore1 = s1_->next(); 
      else if (v1 > v2) hasmore2 = s2_->next();
      else {
        s2_->savePosition();
        joinval_ = s2_->getVal(fldname2_);
        return true;
      }
    }
    return false;
  }
  
  int MergeJoinScan::getInt(const std::string& fldname) {
    if(s1_->hasField(fldname)) return s1_->getInt(fldname);
    return s2_->getInt(fldname);
  }
  
  std::string MergeJoinScan::getString(const std::string& fldname) {
    if(s1_->hasField(fldname)) return s1_->getString(fldname);
    return s2_->getString(fldname);
  }

  scan::Constant MergeJoinScan::getVal(const std::string& fldname) {
    if(s1_->hasField(fldname)) return s1_->getVal(fldname);
    return s2_->getVal(fldname);
  }
  
  bool MergeJoinScan::hasField(const std::string& fldname) {
    return s1_->hasField(fldname) || s2_->hasField(fldname);
  }
}  // namespace materialize
