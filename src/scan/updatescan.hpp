/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include "scan/scan.hpp"
#include "scan/constant.hpp"
#include "record/rid.hpp"

namespace scan {
class UpdateScan: public Scan {
 public:
    virtual ~UpdateScan() = default;
    virtual void setInt(const std::string& fldname, int val) = 0;
    virtual void setString(const std::string& fldname, const std::string& val) = 0;
    virtual void setVal(const std::string& fldname, const Constant& val) = 0;
    virtual void insert() = 0;
    virtual void remove() = 0;
    virtual void moveToRid(const record::RID& rid) = 0;
    virtual record::RID getRid() = 0;
};
}  // namespace scan
