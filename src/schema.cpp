#include "schema.hpp"
#include <algorithm>

namespace record {
  Schema::Schema() {
  }

  Schema& Schema::operator=(const Schema& schema) {
    if(this != &schema) {
      fields_ = schema.fields_;
      info_ = schema.info_;
    }
    return *this;
  }

  void Schema::addField(const std::string& fldname, int type, int length) {
    fields_.emplace_back(fldname);
    info_[fldname] = FieldInfo(type, length);
  }

  void Schema::addIntField(const std::string& fldname) {
    addField(fldname, INTEGER, 0);
  }

  void Schema::addStringField(const std::string& fldname, int length) {
    addField(fldname, VARCHAR, length);
  }

  void Schema::add(const std::string& fldname, const Schema& sch) {
    int type = sch.type(fldname);
    int length = sch.length(fldname);
    addField(fldname, type, length);
  }

  void Schema::addAll(const Schema& sch) {
    for(auto& fldname : sch.fields()) {
      add(fldname, sch);
    }
  }

  std::vector<std::string> Schema::fields() const {
    return fields_;
  }

  bool Schema::hasField(const std::string& fldname) const {
    return std::find(fields_.begin(), fields_.end(), fldname) != fields_.end();
  }

  int Schema::type(const std::string& fldname) const {
    if(info_.find(fldname) == info_.end()) {
      throw std::runtime_error("filed " + fldname + " not found");
    }

    int type = info_.at(fldname).type();
    return type;
  }

  int Schema::length(const std::string& fldname) const {
    if(info_.find(fldname) == info_.end()) {
      throw std::runtime_error("filed " + fldname + " not found");
    }
    int length = info_.at(fldname).length();
    return length;
  }

  Schema::FieldInfo::FieldInfo(int type, int length): type_(type), length_(length) {

  }

  int Schema::FieldInfo::type() const {
    return type_;
  }

  int Schema::FieldInfo::length() const {
    return length_;
  }
}
