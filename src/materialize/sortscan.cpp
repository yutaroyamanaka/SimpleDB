/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/sortscan.hpp"

namespace materialize {
  SortScan::SortScan(const std::vector<std::shared_ptr<TempTable>>& runs, const RecordComparator&comp) : comp_(comp) {
    s1_ = runs[0]->open();
    hasmore1_ = s1_->next();
    if (runs.size() > 1) {
      s2_ = runs[1]->open();
      hasmore2_ = s2_->next();
    }
  }
   
  void SortScan::beforeFirst() {
    s1_->beforeFirst();
    hasmore1_= s1_->next();
    if (s2_) {
      s2_->beforeFirst();
      hasmore2_ = s2_->next();
    } 
  }

  bool SortScan::next() {
    if (currentScan_ == s1_) {
      hasmore1_ = s1_->next();
    } else if (currentScan_ == s2_) {
      hasmore2_ = s2_->next();
    }

    if (!hasmore1_ && !hasmore2_) return false;
    else if (hasmore1_ && hasmore2_) {
      if (comp_.compare(s1_.get(), s2_.get()) < 0) {
        currentScan_ = s1_;
      } else {
        currentScan_ = s2_;
      }
    } else if (hasmore1_) {
      currentScan_ = s1_;
    } else if (hasmore2_) {
      currentScan_ = s2_;
    }
    return true;
  }


  void SortScan::close() {
    s1_->close();
    if (s2_) {
      s2_->close();
    }
  }

  scan::Constant SortScan::getVal(const std::string& fldname) {
    return currentScan_->getVal(fldname);
  }

  int SortScan::getInt(const std::string& fldname) {
    return currentScan_->getInt(fldname);
  }

  std::string SortScan::getString(const std::string& fldname) {
    return currentScan_->getString(fldname);
  }

  bool SortScan::hasField(const std::string& fldname) {
    return currentScan_->hasField(fldname);
  }

  void SortScan::savePosition() {
    record::RID rid1 = s1_->getRid();
    record::RID rid2 = s2_->getRid();
    savedPosition_.emplace_back(rid1);
    savedPosition_.emplace_back(rid2);
  }

  void SortScan::restorePosition() {
    record::RID rid1 = s1_->getRid();
    record::RID rid2 = s2_->getRid();
    savedPosition_.emplace_back(rid1);
    savedPosition_.emplace_back(rid2);
  }
}  // namespace materialize
