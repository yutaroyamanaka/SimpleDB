/* Copyright 2021 Yutaro Yamanaka */
#include "index/btreedir.hpp"

namespace index {
  BTreeDir::BTreeDir(tx::Transaction* transaction, file::BlockId& blk, const record::Layout& layout)
    : transaction_(transaction), layout_(layout) {
      contents_ = BTPage(transaction_, blk, layout_);
      filename_ = blk.fileName();
    }

  void BTreeDir::close() {
    contents_.close();
  }

  int BTreeDir::search(const scan::Constant& searchkey) {
    file::BlockId childblk = findChildBlock(searchkey);
    while (contents_.getFlag() > 0) {
      contents_.close();
      contents_ = BTPage(transaction_, childblk, layout_);
      childblk = findChildBlock(searchkey);
    }
    return childblk.number();
  }

  void BTreeDir::makeNewRoot(const DirEntry& e) {
    scan::Constant firstval = contents_.getDataVal(0);
    int level = contents_.getFlag();
    file::BlockId newblk = contents_.split(0, level);  // ie, transfter all the recs
    DirEntry oldroot = DirEntry(firstval, newblk.number());
    insertEntry(oldroot);
    insertEntry(e);
    contents_.setFlag(level + 1);
  }

  DirEntry BTreeDir::insert(const DirEntry& e) {
    if (contents_.getFlag() == 0) {
      return insertEntry(e);
    }
    file::BlockId childblk = findChildBlock(e.dataVal());
    BTreeDir child(transaction_, childblk, layout_);
    DirEntry myentry = child.insert(e);
    child.close();
    DirEntry de;
    return (!myentry.isNull()) ? insertEntry(myentry) : de;
  }

  DirEntry BTreeDir::insertEntry(const DirEntry& e) {
    int newslot = 1 + contents_.findSlotBefore(e.dataVal());
    contents_.insertDir(newslot, e.dataVal(), e.blockNumber());
    if (!contents_.isFull()) {
      DirEntry de;
      return de;
    }
    // else page is full, so split it
    int level = contents_.getFlag();
    int splitpos = contents_.getNumRecs() / 2;
    scan::Constant splitval = contents_.getDataVal(splitpos);
    file::BlockId newblk = contents_.split(splitpos, level);
    DirEntry de(splitval, newblk.number());
    return de;
  }

  file::BlockId BTreeDir::findChildBlock(const scan::Constant& searchkey) {
    int slot = contents_.findSlotBefore(searchkey);
    if (contents_.getDataVal(slot+1) == searchkey) {  // searchkey is included in slot+1 block
      slot++;
    }
    int blknum = contents_.getChildNum(slot);
    file::BlockId blk(filename_, blknum);
    return blk;
  }
}  // namespace index
