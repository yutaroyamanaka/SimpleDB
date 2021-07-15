#include "recordpage.hpp"
#include "transaction.hpp"
#include "schema.hpp"
#include "layout.hpp"

namespace record {
  RecordPage::RecordPage(tx::Transaction* transaction, const file::BlockId& block_id, const Layout& layout) : tx_(transaction), block_id_(block_id), layout_(layout) {
    tx_->pin(block_id_);
  }

  int RecordPage::getInt(int slot, const std::string& fldname) {
    int fldpos = offset(slot) + layout_.offset(fldname);
    return tx_->getInt(block_id_, fldpos);
  }

  std::string RecordPage::getString(int slot, const std::string& fldname) {
    int fldpos = offset(slot) + layout_.offset(fldname);
    return tx_->getString(block_id_, fldpos);
  }

  void RecordPage::setInt(int slot, const std::string& fldname, int val) {
    int fldpos = offset(slot) + layout_.offset(fldname);
    tx_->setInt(block_id_, fldpos, val, true);
  }

  void RecordPage::setString(int slot, const std::string& fldname, const std::string& val) {
    int fldpos = offset(slot) + layout_.offset(fldname);
    tx_->setString(block_id_, fldpos, val, true);
  }

  void RecordPage::remove(int slot) {
    setFlag(slot, EMPTY);
  }

  void RecordPage::format() {
    int slot = 0;
    while(isValidSlot(slot)) {
      tx_->setInt(block_id_, offset(slot), EMPTY, false);
      Schema sch = layout_.schema();
      for(auto& fldname: sch.fields()) {
        int fldpos = offset(slot) + layout_.offset(fldname);
        if(sch.type(fldname) == Schema::INTEGER) {
          tx_->setInt(block_id_, fldpos, 0, false);
        } else {
          tx_->setString(block_id_, fldpos, "", false);
        }
        slot++;
      }
    }
  }

  int RecordPage::nextAfter(int slot) {
    return searchAfter(slot, USED);
  }

  int RecordPage::insertAfter(int slot) {
    int newslot = searchAfter(slot, EMPTY);
    if(newslot >= 0) {
      setFlag(newslot, USED);
    }
    return newslot;
  }

  file::BlockId RecordPage::block() const {
    return block_id_;
  }

  void RecordPage::setFlag(int slot, int flag) {
    tx_->setInt(block_id_, offset(slot), flag, true);
  }

  int RecordPage::searchAfter(int slot, int flag) {
    slot++;
    while(isValidSlot(slot)) {
      if(tx_->getInt(block_id_, offset(slot)) == flag) {
        return slot;
      }
      slot++;
    }
    return -1;
  }

  bool RecordPage::isValidSlot(int slot) {
    return offset(slot+1) <= tx_->blockSize();
  }

  int RecordPage::offset(int slot) {
    return slot * layout_.slotSize();
  }
}
