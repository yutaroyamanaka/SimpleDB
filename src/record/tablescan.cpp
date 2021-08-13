/* Copyright 2021 Yutaro Yamanaka */
#include "record/tablescan.hpp"
#include "record/layout.hpp"

namespace record {
  TableScan::~TableScan() {
  }

  TableScan::TableScan(tx::Transaction* transaction, const std::string& tblname, const Layout& layout) : tx_(transaction), layout_(layout) {
    filename_ = tblname + ".tbl";
    if (tx_->size(filename_) == 0) {
      moveToNewBlock();
    } else {
      moveToBlock(0);
    }
  }

  void TableScan::close() {
    if (record_page_) {
      tx_->unpin(record_page_->block());
    }
  }

  void TableScan::beforeFirst() {
    moveToBlock(0);
  }

  bool TableScan::next() {
    current_slot_ = record_page_->nextAfter(current_slot_);
    while (current_slot_ < 0) {
      if (atLastBlock()) return false;
      moveToBlock(record_page_->block().number() + 1);
      current_slot_ = record_page_->nextAfter(current_slot_);
    }
    return true;
  }

  int TableScan::getInt(const std::string& fldname) {
    return record_page_->getInt(current_slot_, fldname);
  }

  std::string TableScan::getString(const std::string& fldname) {
    return record_page_->getString(current_slot_, fldname);
  }

  scan::Constant TableScan::getVal(const std::string& fldname) {
    if (layout_.schema().type(fldname) == Schema::INTEGER) {
      return scan::Constant(getInt(fldname));
    } else {
      return scan::Constant(getString(fldname));
    }
  }

  bool TableScan::hasField(const std::string& fldname) {
    return layout_.schema().hasField(fldname);
  }

  void TableScan::setInt(const std::string& fldname, int val) {
    record_page_->setInt(current_slot_, fldname, val);
  }

  void TableScan::setString(const std::string& fldname, const std::string& val) {
    record_page_->setString(current_slot_, fldname, val);
  }

  void TableScan::setVal(const std::string& fldname, const scan::Constant& val) {
    if (layout_.schema().type(fldname) == Schema::INTEGER) {
      setInt(fldname, val.asInt());
    } else {
      setString(fldname, val.asString());
    }
  }

  void TableScan::insert() {
    current_slot_ = record_page_->insertAfter(current_slot_);
    while (current_slot_ < 0) {
      if (atLastBlock()) {
        moveToNewBlock();
      } else {
        moveToBlock(record_page_->block().number()+1);
      }
      current_slot_ = record_page_->insertAfter(current_slot_);
    }
  }

  void TableScan::remove() {
    record_page_->remove(current_slot_);
  }

  void TableScan::moveToRid(const RID& rid) {
    close();
    file::BlockId block_id(filename_, rid.blockNumber());
    record_page_ = std::make_unique<RecordPage>(tx_, block_id, layout_);
    current_slot_ = rid.slot();
  }

  RID TableScan::getRid() {
    RID rid(record_page_->block().number(), current_slot_);
    return rid;
  }

  void TableScan::moveToBlock(int blknum) {
    close();
    file::BlockId block_id(filename_, blknum);
    record_page_ = std::make_unique<RecordPage>(tx_, block_id, layout_);
    current_slot_ = -1;
  }

  void TableScan::moveToNewBlock() {
    close();
    file::BlockId block_id = tx_->append(filename_);
    record_page_ = std::make_unique<RecordPage>(tx_, block_id, layout_);
    record_page_->format();
    current_slot_ = -1;
  }

  bool TableScan::atLastBlock() {
    return record_page_->block().number() == tx_->size(filename_) - 1;
  }
}  // namespace record
