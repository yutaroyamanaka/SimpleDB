/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <scan/constant.hpp>

namespace scan {
class Scan {
 public:
    virtual ~Scan() = default;
    virtual void beforeFirst() = 0;
    virtual bool next() = 0;
    virtual int getInt(const std::string& fldname) = 0;
    virtual std::string getString(const std::string& fldname) = 0;
    virtual Constant getVal(const std::string& fldname) = 0;
    virtual bool hasField(const std::string& fldname) = 0;
    virtual void close() = 0;
};
}  // namespace scan
