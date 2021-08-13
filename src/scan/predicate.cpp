/* Copyright 2021 Yutaro Yamanaka */
#include "scan/predicate.hpp"

namespace scan {
  Predicate::Predicate() {
  }

  Predicate::Predicate(const Predicate& pred): terms_(pred.terms_) {
  }

  Predicate::Predicate(const Term& t) {
    terms_.emplace_back(t);
  }

  Predicate &Predicate::operator=(const Predicate& pred) {
    if (this != &pred) {
      terms_ = pred.terms_;
    }
    return *this;
  }

  void Predicate::conjoinWith(const Predicate& pred) {
    terms_.insert(terms_.end(), pred.terms_.begin(), pred.terms_.end());
  }

  bool Predicate::isSatisfied(Scan* s) const {
    for (auto& t : terms_) {
      if (!t.isSatisfied(s)) {
        return false;
      }
    }
    return true;
  }

  int Predicate::reductionFacotr() const {
    return -1;
  }

  Predicate Predicate::selectSubPred(const record::Schema& sch) const {
    Predicate result;
    for (auto& t : terms_) {
      if (t.appliesTo(sch)) {
        result.terms_.emplace_back(t);
      }
    }
    return result;
  }

  Predicate Predicate::joinSubPred(const record::Schema& sch1, const record::Schema& sch2) const {
    Predicate result;
    record::Schema newsch;
    newsch.addAll(sch1);
    newsch.addAll(sch2);
    for (auto& t : terms_) {
      if (!t.appliesTo(sch1) && !t.appliesTo(sch2) && t.appliesTo(newsch)) {
        result.terms_.emplace_back(t);
      }
    }
    return result;
  }

  Constant Predicate::equatesWithConstant(const std::string& fldname) const {
    for (auto& t : terms_) {
      Constant c = t.equatesWithConstant(fldname);
      if (!c.isNull()) return c;
    }
    Constant n;
    return n;
  }

  std::string Predicate::equatesWithField(const std::string& fldname) const {
    for (auto& t : terms_) {
      std::string s = t.equatesWithField(fldname);
      if (!s.empty()) return s;
    }
    return "";
  }

  std::string Predicate::toString() const {
    std::string result = "";
    int cnt = 0;
    for (auto& t : terms_) {
      if (cnt == 0) {
        result += t.toString();
      } else {
        result += " and " + t.toString();
      }
      cnt++;
    }
    return result;
  }
}  // namespace scan
