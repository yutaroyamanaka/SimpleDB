/* Copyright 2021 Yutaro Yamanaka */
#include "scan/constant.hpp"

namespace scan {
  bool operator==(const Constant& lhs, const Constant& rhs) {
    return lhs.ival_ ? *(lhs.ival_) == *(rhs.ival_) : *(lhs.sval_) == *(rhs.sval_);
  }

  bool operator!=(const Constant& lhs, const Constant& rhs) {
    return !(lhs == rhs);
  }

  bool operator<(const Constant& lhs, const Constant& rhs) {
    return lhs.ival_ ? *(lhs.ival_) < *(rhs.ival_) : *(lhs.sval_) < *(rhs.sval_);
  }

  bool operator>(const Constant& lhs, const Constant& rhs) {
    return lhs.ival_ ? *(lhs.ival_) > *(rhs.ival_) : *(lhs.sval_) > *(rhs.sval_);
  }

  bool operator<=(const Constant& lhs, const Constant& rhs) {
    return !(lhs > rhs);
  }

  bool operator>=(const Constant& lhs, const Constant& rhs) {
    return !(lhs < rhs);
  }

  Constant::Constant() {
  }

  Constant::Constant(const Constant& val) {
    if (val.ival_) {
      ival_ = std::make_unique<int>(*(val.ival_));
    }
    if (val.sval_) {
      sval_ = std::make_unique<std::string>(*(val.sval_));
    }
  }

  Constant::Constant(int ival) {
    ival_ = std::make_unique<int>(ival);
  }

  Constant::Constant(const std::string& sval) {
    sval_ = std::make_unique<std::string>(sval);
  }

  Constant &Constant::operator=(const Constant& val) {
    if (this != &val) {
      if (val.ival_) {
        ival_ = std::make_unique<int>(*(val.ival_));
      }
      if (val.sval_) {
        sval_ = std::make_unique<std::string>(*(val.sval_));
      }
    }
    return *this;
  }

  int Constant::asInt() const {
    return *(ival_);
  }

  std::string Constant::asString() const {
    return *(sval_);
  }

  int Constant::hashCode() const {
    return (ival_ ? std::hash<int>{}(*ival_) : std::hash<std::string>{}(*sval_));
  }

  std::string Constant::toString() const {
    return (ival_ ? std::to_string(*ival_) : *(sval_));
  }

  bool Constant::isNull() const {
    if (ival_ || sval_) return false;
    return true;
  }
}  // namespace scan
