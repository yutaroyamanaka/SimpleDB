/* Copyright 2021 Yutaro Yamanaka */
#include "interface/resultset.hpp"

namespace interface {
  ResultSet::ResultSet(plan::Plan* p, Connection* conn) : s_(p->open()), sch_(p->schema()), conn_(conn) {
  }

  bool ResultSet::next() {
    try {
      return s_->next();
    } catch (std::exception& e) {
      conn_->rollback();
      std::runtime_error("cannot execute ResultSet::next");
      return false;
    }
  }

  int ResultSet::getInt(std::string& fldname) const {
    try {
      boost::algorithm::to_lower(fldname);
      return s_->getInt(fldname);
    } catch (std::exception& e) {
      conn_->rollback();
      std::runtime_error("cannot execute ResultSet::getInt");
      return -1;
    }
  }

  std::string ResultSet::getString(std::string& fldname) const {
    try {
      boost::algorithm::to_lower(fldname);
      return s_->getString(fldname);
    } catch (std::exception& e) {
      conn_->rollback();
      std::runtime_error("cannot execute ResultSet::getString");
      return "";
    }
  }

  MetaData ResultSet::getMetaData() const {
    MetaData md(sch_);
    return md;
  }

  void ResultSet::close() {
    try {
      s_->close();
      conn_->commit();
    } catch (std::exception& e) {
      s_->close();
      conn_->commit();
    }
  }
}  // namespace interface
