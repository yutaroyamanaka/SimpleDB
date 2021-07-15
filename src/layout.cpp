#include "layout.hpp"
#include "schema.hpp"
#include "page.hpp"

namespace record {
  Layout::Layout() {

  }

  Layout::Layout(const Schema& schema) : schema_(schema) {
    int pos = sizeof(uint32_t);
    for(auto& fldname: schema_.fields()) {
      offsets_[fldname] = pos;
      pos += lengthInBytes(fldname);
    }
    slotsize_ = pos;
  }

  Layout::Layout(const Schema& schema, std::map<std::string, int>& offsets, int slotsize) : schema_(schema), offsets_(offsets), slotsize_(slotsize) {

  }

  Schema Layout::schema() {
    return schema_;
  }

  int Layout::offset(const std::string& fldname) {
    return offsets_[fldname];
  }

  int Layout::slotSize() {
    return slotsize_;
  }

  int Layout::lengthInBytes(const std::string& fldname) {
    int fldtype = schema_.type(fldname);
    if(fldtype == Schema::INTEGER) {
      return static_cast<int>(sizeof(uint32_t));
    } else {
      return file::Page::maxLength(schema_.length(fldname));
    }
  }
}
