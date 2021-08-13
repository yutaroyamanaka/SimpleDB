/* Copyright 2021 Yutaro Yamanaka */
#include "layout.hpp"
#include "schema.hpp"
#include "page.hpp"

namespace record {
  Layout::Layout() {
  }

  Layout::Layout(const Schema& schema) : schema_(schema) {
    int pos = static_cast<int>(sizeof(uint32_t));
    for (const auto& fldname : schema_.fields()) {
      offsets_[fldname] = pos;
      pos += lengthInBytes(fldname);
    }
    slotsize_ = pos;
  }

  Layout::Layout(const Schema& schema, std::map<std::string, int>& offsets, int slotsize) : schema_(schema), offsets_(offsets), slotsize_(slotsize) {
  }

  Layout &Layout::operator=(const Layout& layout) {
    if (this != &layout) {
      schema_ = layout.schema_;
      offsets_ = layout.offsets_;
      slotsize_ = layout.slotsize_;
    }
    return *this;
  }

  Schema Layout::schema() const {
    return schema_;
  }

  int Layout::offset(const std::string& fldname) const {
    if (offsets_.find(fldname) == offsets_.end()) {
      throw std::runtime_error("filed name" + fldname + " not found");
    }
    return offsets_.at(fldname);
  }

  int Layout::slotSize() const {
    return slotsize_;
  }

  int Layout::lengthInBytes(const std::string& fldname) {
    int fldtype = schema_.type(fldname);
    if (fldtype == Schema::INTEGER) {
      return static_cast<int>(sizeof(uint32_t));
    } else if (fldtype == Schema::VARCHAR) {
      return file::Page::maxLength(schema_.length(fldname));
    }

    throw std::runtime_error("filed type not defined");
  }
}  // namespace record
