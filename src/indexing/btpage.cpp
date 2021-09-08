/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/btpage.hpp"

namespace indexing {
  BTPage::BTPage() {
  }

  BTPage::BTPage(tx::Transaction* transaction, const file::BlockId& currentblk, const record::Layout& layout)
    : transaction_(transaction), currentblk_(currentblk), layout_(layout) {
    transaction_->pin(currentblk_);
  }

  int BTPage::findSlotBefore(const scan::Constant& searchkey) {
    int slot = 0;
    while (slot < getNumRecs() && getDataVal(slot) < searchkey) {
      slot++;
    }
    return slot-1;
  }

  void BTPage::close() {
    if (!currentblk_.isNull()) {
      transaction_->unpin(currentblk_);
    }
    currentblk_ = file::BlockId();
  }

  bool BTPage::isFull() {
    return slotpos(getNumRecs() + 1) >= transaction_->blockSize();
  }

  file::BlockId BTPage::split(int splitpos, int flag) {
    file::BlockId newblk = appendNew(flag);
    BTPage newpage(transaction_, newblk, layout_);
    transferRecs(splitpos, newpage);
    newpage.setFlag(flag);  // set layer number
    newpage.close();
    return newblk;
  }

  scan::Constant BTPage::getDataVal(int slot) {
    return getVal(slot, "dataval");
  }

  int BTPage::getFlag() {
    return transaction_->getInt(currentblk_, 0);
  }

  void BTPage::setFlag(int val) {
    transaction_->setInt(currentblk_, 0, val, true);
  }

  file::BlockId BTPage::appendNew(int flag) {
    file::BlockId blk = transaction_->append(currentblk_.fileName());
    transaction_->pin(blk);
    format(blk, flag);
    return blk;
  }

  void BTPage::format(const file::BlockId& blk, int flag) {
    transaction_->setInt(blk, 0, flag, false);  // layer number
    transaction_->setInt(blk, sizeof(uint32_t), 0, false);  // #records = 0
    int recsize = layout_.slotSize();
    for (int pos = 2 * sizeof(uint32_t); pos+recsize <= transaction_->blockSize(); pos += recsize) {
      makeDefaultRecord(blk, pos);
    }
  }

  void BTPage::makeDefaultRecord(const file::BlockId& blk, int pos) {
    for (const auto& fldname : layout_.schema().fields()) {
      int offset = layout_.offset(fldname);
      if (layout_.schema().type(fldname) == record::Schema::INTEGER) {
        transaction_->setInt(blk, pos + offset, 0, false);
      } else {
        transaction_->setString(blk, pos + offset, "", false);
      }
    }
  }

  // Method called only by BTreeDir
  int BTPage::getChildNum(int slot) {
    return getInt(slot, "block");
  }

  void BTPage::insertDir(int slot, const scan::Constant& val, int blknum) {
    insert(slot);
    setVal(slot, "dataval", val);
    setInt(slot, "block", blknum);
  }

  // Method called only by BTreeLeaf
  record::RID BTPage::getDataRid(int slot) {
    record::RID rid(getInt(slot, "block"), getInt(slot, "id"));
    return rid;
  }

  void BTPage::insertLeaf(int slot, const scan::Constant& val, const record::RID& rid) {
    insert(slot);
    setVal(slot, "dataval", val);
    setInt(slot, "block", rid.blockNumber());
    setInt(slot, "id", rid.slot());
  }

  void BTPage::remove(int slot) {
    for (int i = slot + 1; i < getNumRecs(); i++) {
      copyRecord(i, i-1);
    }
    setNumRecs(getNumRecs()-1);
    return;
  }

  int BTPage::getNumRecs() {
    return transaction_->getInt(currentblk_, sizeof(uint32_t));
  }

  int BTPage::getInt(int slot, const std::string& fldname) {
    int pos = fldpos(slot, fldname);
    return transaction_->getInt(currentblk_, pos);
  }

  std::string BTPage::getString(int slot, const std::string& fldname) {
    int pos = fldpos(slot, fldname);
    return transaction_->getString(currentblk_, pos);
  }

  scan::Constant BTPage::getVal(int slot, const std::string& fldname) {
    int type = layout_.schema().type(fldname);
    if (type == record::Schema::INTEGER) {
      scan::Constant c(getInt(slot, fldname));
      return c;
    } else {
      scan::Constant c(getString(slot, fldname));
      return c;
    }
  }

  void BTPage::setInt(int slot, const std::string& fldname, int val) {
    int pos = fldpos(slot, fldname);
    transaction_->setInt(currentblk_, pos, val, true);
  }

  void BTPage::setString(int slot, const std::string& fldname, const std::string& val) {
    int pos = fldpos(slot, fldname);
    transaction_->setString(currentblk_, pos, val, true);
  }

  void BTPage::setVal(int slot, const std::string& fldname, const scan::Constant& val) {
    int type = layout_.schema().type(fldname);
    if (type == record::Schema::INTEGER) {
      setInt(slot, fldname, val.asInt());
    } else {
      setString(slot, fldname, val.asString());
    }
  }

  void BTPage::setNumRecs(int n) {
    transaction_->setInt(currentblk_, sizeof(uint32_t), n, true);
  }

  void BTPage::insert(int slot) {
    for (int i = getNumRecs(); i > slot; i--) {
      copyRecord(i-1, i);
    }
    setNumRecs(getNumRecs()+1);
  }

  void BTPage::copyRecord(int from, int to) {
    record::Schema sch = layout_.schema();
    for (const auto& fldname : sch.fields()) {
      setVal(to, fldname, getVal(from, fldname));
    }
  }

  void BTPage::transferRecs(int slot, BTPage& dest) {
    int destslot = 0;
    while (slot < getNumRecs()) {
      dest.insert(destslot);  // only call setNumRecs ?
      record::Schema sch = layout_.schema();
      for (const auto& fldname : sch.fields()) {
        dest.setVal(destslot, fldname, getVal(slot, fldname));
      }
      remove(slot);
      destslot++;
    }
  }

  int BTPage::fldpos(int slot, const std::string& fldname) {
    int offset = layout_.offset(fldname);
    return slotpos(slot) + offset;
  }

  int BTPage::slotpos(int slot) {
    int slotsize = layout_.slotSize();
    return sizeof(uint32_t) + sizeof(uint32_t) + (slot * slotsize);
  }

  bool BTPage::isNull() {
    return transaction_ == nullptr;
  }
}  // namespace indexing
