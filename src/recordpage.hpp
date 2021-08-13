/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <map>
#include <string>
#include "transaction.hpp"
#include "blockid.hpp"
#include "layout.hpp"

namespace record {
class RecordPage {
 public:
    RecordPage(tx::Transaction* transaction, const file::BlockId& block_id, const Layout& layout);
    int getInt(int slot, const std::string& fldname) const;
    std::string getString(int slot, const std::string& fldname) const;
    void setInt(int slot, const std::string& fldname, int val);
    void setString(int slot, const std::string& fldname, const std::string& val);
    void remove(int slot);
    void format();
    int nextAfter(int slot);
    int insertAfter(int slot);
    file::BlockId block() const;
    static const int EMPTY = 0, USED = 1;
 private:
    tx::Transaction* tx_;
    file::BlockId block_id_;
    Layout layout_;

    void setFlag(int slot, int flag);
    int searchAfter(int slot, int flag);
    bool isValidSlot(int slot) const;
    int offset(int slot) const;
};
}  // namespace record
