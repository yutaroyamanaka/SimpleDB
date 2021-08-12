/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include "schema.hpp"
#include <vector>
#include <map>
#include <string>

namespace record {
  class Layout {
    public:
      Layout();
      Layout(const Schema& schema);
      Layout(const Schema& schema, std::map<std::string, int>& offsets, int slotsize);
      Layout &operator=(const Layout& layout);
      Schema schema() const;
      int offset(const std::string& fldname) const;
      int slotSize() const;
    private:
      Schema schema_;
      std::map<std::string, int> offsets_;
      int slotsize_;
      int lengthInBytes(const std::string& fldname);
  };
}
