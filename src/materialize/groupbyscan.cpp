/* Copyright 2021 Yutaro Yamanaka */
#include "materialize/groupbyscan.hpp"

namespace materialize {
  GroupByScan::GroupByScan(const std::shared_ptr<scan::Scan>& s, const std::vector<std::string>& groupFields,
      const std::vector<std::shared_ptr<AggregationFn>>& aggfns) :
    s_(s), groupFields_(groupFields), aggfns_(aggfns) {
      beforeFirst();
    }

  void GroupByScan::beforeFirst() {
    s_->beforeFirst();
    moreGroups_ = s_->next();
  }

  bool GroupByScan::next() {
    if (!moreGroups_) return false;
    for (const auto& ptr : aggfns_) ptr->processFirst(s_.get());
    groupVal_ = std::make_unique<GroupValue>(s_.get(), groupFields_);
    while (moreGroups_ == s_->next()) {
      auto gv = std::make_unique<GroupValue>(s_.get(), groupFields_);
      if (*groupVal_ != *gv) break;
      for (const auto& fn : aggfns_) {
        fn->processNext(s_.get());
      }
    }
    return true;
  }

  void GroupByScan::close() {
    s_->close();
  }

  scan::Constant GroupByScan::getVal(const std::string& fldname) {
    if (std::find(groupFields_.begin(), groupFields_.end(), fldname) != groupFields_.end()) {
      return groupVal_->getVal(fldname);
    }
    for (const auto& fn : aggfns_) {
      if (fn->fieldName() == fldname) return fn->value();
    }
    throw std::runtime_error("filed " + fldname + " not found.");
  }

  int GroupByScan::getInt(const std::string& fldname) {
    return getVal(fldname).asInt();
  }

  std::string GroupByScan::getString(const std::string& fldname) {
    return getVal(fldname).asString();
  }

  bool GroupByScan::hasField(const std::string& fldname) {
    if (std::find(groupFields_.begin(), groupFields_.end(), fldname) != groupFields_.end()) {
      return true;
    }
    for (const auto& fn : aggfns_) {
      if (fn->fieldName() == fldname) return true;
    }
    return false;
  }
}  // namespace materialize
