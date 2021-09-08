/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include "scan/constant.hpp"

namespace indexing {
class DirEntry {
 public:
    DirEntry();
    DirEntry(const scan::Constant& dataval, int blocknum);
    scan::Constant dataVal() const;
    int blockNumber() const;
    bool isNull() const;
 private:
    scan::Constant dataval_;
    int blocknum_;
};
}  // namespace indexing
