/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <map>
#include <string>

namespace record {
  class Schema {
    public:
      Schema();
      Schema &operator=(const Schema& schema);
      void addField(const std::string& fldname, int type, int length);
      void addIntField(const std::string& fldname);
      void addStringField(const std::string& fldname, int length);
      void add(const std::string& fldname, const Schema& sch);
      void addAll(const Schema& sch);
      std::vector<std::string> fields() const;
      bool hasField(const std::string& fldname) const;
      int type(const std::string& fldname) const;
      int length(const std::string& fldname) const;
      class FieldInfo {
        public:
          FieldInfo() = default;
          FieldInfo(int type, int length);
          int type() const;
          int length() const;
        private:
          int type_;
          int length_;
      };
      static const int INTEGER = 0, VARCHAR = 1;
    private:
      std::vector<std::string> fields_;
      std::map<std::string, FieldInfo> info_;
  };
}
