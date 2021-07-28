#include "indexmanager.hpp"

namespace meta {
  IndexManager::IndexManager(bool isNew, TableManager* table_manager, StatManager* stat_manager, tx::Transaction* transaction) : table_manager_(table_manager), stat_manager_(stat_manager) {
    if(isNew) {
      record::Schema sch;
      sch.addStringField("indexname", TableManager::MAX_NAME);
      sch.addStringField("tablename", TableManager::MAX_NAME);
      sch.addStringField("fieldname", TableManager::MAX_NAME);
      table_manager_->createTable("idxcat", sch, transaction);
    }

    layout_ = table_manager_->getLayout("idxcat", transaction);
  }

  void IndexManager::createIndex(const std::string& idxname, const std::string& tblname, const std::string& fldname, tx::Transaction* transaction) {
    record::TableScan ts(transaction, "idxcat", layout_);
    ts.insert();
    ts.setString("indexname", idxname);
    ts.setString("tablename", tblname);
    ts.setString("fieldname", fldname);
    ts.close();
  }

  std::map<std::string, IndexInfo> IndexManager::getIndexInfo(const std::string& tblname, tx::Transaction* transaction) {
    std::map<std::string, IndexInfo> result;
    record::TableScan ts(transaction, "idxcat", layout_);
    while(ts.next()) {
      if(ts.getString("tablename") == tblname) {
        std::string idxname = ts.getString("indexname");
        std::string fldname = ts.getString("fieldname");
        record::Layout tbllayout = table_manager_->getLayout(tblname, transaction);
        StatInfo tblsi = stat_manager_->getStatInfo(tblname, tbllayout, transaction);
        IndexInfo ii(idxname, fldname, tbllayout.schema(), transaction, tblsi);
        result[fldname] = ii;
      }
    }
    ts.close();
    return result;
  }

  IndexInfo::IndexInfo() {

  }

  IndexInfo::IndexInfo(const std::string& idxname, const std::string& fldname, const record::Schema& schema, tx::Transaction* transaction, const StatInfo& si) : idxname_(idxname), fldname_(fldname), transaction_(transaction), schema_(schema), si_(si) {
    layout_ = createIdxLayout();
  }

  void IndexInfo::open() {
    record::Schema sch;
    return;
  }

  int IndexInfo::blocksAccessed() {
    int rpb = transaction_->blockSize() / layout_.slotSize();
    int num_blocks = si_.recordOutput() / rpb;
    return 0;
  }

  int IndexInfo::recordOutput() {
    return si_.recordOutput() / si_.distinctValues(fldname_);
  }

  int IndexInfo::distinctValues(const std::string& fname) {
    return fldname_ == fname ? 1 : si_.distinctValues(fldname_);
  }

  record::Layout IndexInfo::createIdxLayout() {
    record::Schema sch;
    sch.addIntField("block");
    sch.addIntField("id");
    if(layout_.schema().type(fldname_) == record::Schema::INTEGER) {
      sch.addIntField("dataval");
    } else {
      int fldlen = layout_.schema().length(fldname_);
      sch.addStringField("dataval", fldlen);
    }
    record::Layout layout(sch);
    return layout;
  }
}
