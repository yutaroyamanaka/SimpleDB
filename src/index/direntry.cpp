/* Copyright 2021 Yutaro Yamanaka */
#include "index/direntry.hpp"

namespace index {
  DirEntry::DirEntry() {
  }

  DirEntry::DirEntry(const scan::Constant& dataval, int blocknum) : dataval_(dataval), blocknum_(blocknum) {
  }

  scan::Constant DirEntry::dataVal() const {
    return dataval_;
  }

  int DirEntry::blockNumber() const {
    return blocknum_;
  }

  bool DirEntry::isNull() const {
    return dataval_.isNull();
  }
}  // namespace index
