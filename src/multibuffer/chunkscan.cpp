/* Copyright 2021 Yutaro Yamanaka */
#include "multibuffer/chunkscan.hpp"

namespace multibuffer {
  ChunkScan::ChunkScan(tx::Transaction* transaction, const std::string& filename, const record::Layout& layout, int startbnum, int endbnum)
    : transaction_(transaction), filename_(filename), layout_(layout), startbnum_(startbnum), endbnum_(endbnum) {
      for (int i = startbnum_; i <= endbnum_; i++) {
        file::BlockId blk(filename_, i);
        buffs_.emplace_back(std::make_unique<record::RecordPage>(transaction_, blk, layout_));
      }
      moveToBlock(startbnum_);
    }

  void ChunkScan::close() {
    for (int i = 0; i < buffs_.size(); i++) {
      file::BlockId blk(filename_, startbnum_+i);
      transaction_->unpin(blk);
    }
  }

  void ChunkScan::beforeFirst() {
    moveToBlock(startbnum_);
  }

  bool ChunkScan::next() {
    currentslot_ = rp_->nextAfter(currentslot_);
    while (currentslot_ < 0) {
      if (currentbnum_ == endbnum_) return false;
      moveToBlock(rp_->block().number()+1);
      currentslot_ = rp_->nextAfter(currentslot_);
    }
    return true;
  }

  int ChunkScan::getInt(const std::string& fldname) {
    return rp_->getInt(currentslot_, fldname);
  }

  std::string ChunkScan::getString(const std::string& fldname) {
    return rp_->getString(currentslot_, fldname);
  }

  scan::Constant ChunkScan::getVal(const std::string& fldname) {
    if (layout_.schema().type(fldname) == record::Schema::INTEGER) {
      return scan::Constant(getInt(fldname));
    } else {
      return scan::Constant(getString(fldname));
    }
  }

  bool ChunkScan::hasField(const std::string& fldname) {
    return layout_.schema().hasField(fldname);
  }

  void ChunkScan::moveToBlock(int blknum) {
    currentbnum_ = blknum;
    rp_ = buffs_[currentbnum_ - startbnum_].get();
    currentslot_ = -1;
  }
}  // namespace multibuffer
