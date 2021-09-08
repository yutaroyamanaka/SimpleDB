/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include "scan/constant.hpp"
#include "record/rid.hpp"

namespace indexing {
class Index {
 public:
    virtual ~Index() = default;
    virtual void beforeFirst(const scan::Constant& searchkey) = 0;
    virtual bool next() = 0;
    virtual record::RID getDataRid() = 0;
    virtual void insert(const scan::Constant& dataval, const record::RID& datarid) = 0;
    virtual void remove(const scan::Constant& dataval, const record::RID& datarid) = 0;
    virtual void close() = 0;
};
}  // namespace indexing
