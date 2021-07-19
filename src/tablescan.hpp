#pragma once
#include "transaction.hpp"
#include "layout.hpp"
#include "recordpage.hpp"
#include "rid.hpp"
#include <string>
#include <memory>

namespace record {
  class TableScan {
    public:
      TableScan();
      TableScan(tx::Transaction* transaction, const std::string& tblname, const Layout& layout);
      void close();
      void beforeFirst();
      bool next();
      int getInt(const std::string& fldname);
      std::string getString(const std::string& fldname);
      bool hasField(const std::string& fldname);
      void setInt(const std::string& fldname, int val);
      void setString(const std::string& fldname, const std::string& val);
      void insert();
      void remove();
      void moveToRid(const RID& rid);
      RID getRid();
    private:
      void moveToBlock(int blknum);
      void moveToNewBlock();
      bool atLastBlock();

      tx::Transaction* tx_;
      Layout layout_;
      std::unique_ptr<RecordPage> record_page_;
      std::string filename_;
      int current_slot_;
  };
}
