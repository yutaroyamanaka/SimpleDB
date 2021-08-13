/* Copyright 2021 Yutaro Yamanaka */
#include "selectscan.hpp"

namespace scan {
  SelectScan::SelectScan(std::shared_ptr<Scan>& s, const Predicate& pred) : s_(s), pred_(pred) {
  }

  void SelectScan::beforeFirst() {
    s_->beforeFirst();
  }

  bool SelectScan::next() {
    while (s_->next()) {
      if (pred_.isSatisfied(s_.get())) return true;
    }
    return false;
  }

  int SelectScan::getInt(const std::string& fldname) {
    return s_->getInt(fldname);
  }

  std::string SelectScan::getString(const std::string& fldname) {
    return s_->getString(fldname);
  }

  Constant SelectScan::getVal(const std::string& fldname) {
    return s_->getVal(fldname);
  }

  bool SelectScan::hasField(const std::string& fldname) {
    return s_->hasField(fldname);
  }

  void SelectScan::close() {
    s_->close();
  }

  void SelectScan::setInt(const std::string& fldname, int val) {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      us->setInt(fldname, val);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void SelectScan::setString(const std::string& fldname, const std::string& val) {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      us->setString(fldname, val);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void SelectScan::setVal(const std::string& fldname, const Constant& val) {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      us->setVal(fldname, val);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void SelectScan::remove() {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      us->remove();
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void SelectScan::insert() {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      us->insert();
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  record::RID SelectScan::getRid() {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      return us->getRid();
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void SelectScan::moveToRid(const record::RID& rid) {
    std::shared_ptr<UpdateScan> us = std::dynamic_pointer_cast<UpdateScan>(s_);
    if (us) {
      return us->moveToRid(rid);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }
}  // namespace scan
