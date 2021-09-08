/* Copyright 2021 Yutaro Yamanaka */
#include "indexing/btreeleaf.hpp"

namespace indexing {
  BTreeLeaf::BTreeLeaf() {
  }

  BTreeLeaf::BTreeLeaf(tx::Transaction* transaction, file::BlockId& blk, const record::Layout& layout, const scan::Constant& searchkey)
    : transaction_(transaction), layout_(layout), searchkey_(searchkey) {
      contents_ = BTPage(transaction_, blk, layout_);
      currentslot_ = contents_.findSlotBefore(searchkey_);
      filename_ = blk.fileName();
    }

  void BTreeLeaf::close() {
    contents_.close();
  }

  bool BTreeLeaf::next() {
    currentslot_++;
    if (currentslot_ >= contents_.getNumRecs()) {
      return tryOverflow();
    } else if (contents_.getDataVal(currentslot_) == searchkey_) {
      return true;
    }
    return tryOverflow();
  }

  record::RID BTreeLeaf::getDataRid() {
    return contents_.getDataRid(currentslot_);
  }

  void BTreeLeaf::remove(const record::RID& datarid) {
    while (next()) {
      if (getDataRid().equals(datarid)) {
        contents_.remove(currentslot_);
        return;
      }
    }
  }

  DirEntry BTreeLeaf::insert(const record::RID& datarid) {
    if (contents_.getFlag() >= 0 && contents_.getDataVal(0) > searchkey_) {
      // page is stil empty
      scan::Constant firstval = contents_.getDataVal(0);
      file::BlockId newblk = contents_.split(0, contents_.getFlag());
      currentslot_ = 0;
      contents_.setFlag(-1);
      contents_.insertLeaf(currentslot_, searchkey_, datarid);
      DirEntry de(firstval, newblk.number());
      return de;
    }
    currentslot_++;
    contents_.insertLeaf(currentslot_, searchkey_, datarid);
    if (!contents_.isFull()) {
      DirEntry de;
      return de;
    }
    // else page is full, so split it
    scan::Constant firstkey = contents_.getDataVal(0);
    scan::Constant lastkey = contents_.getDataVal(contents_.getNumRecs()-1);
    if (lastkey == firstkey) {
      // create an overflow block to hold all but the first record
      file::BlockId newblk = contents_.split(1, contents_.getFlag());
      contents_.setFlag(newblk.number());
      DirEntry de;
      return de;
    } else {
      int splitpos = contents_.getNumRecs() / 2;
      scan::Constant splitkey = contents_.getDataVal(splitpos);
      if (splitkey == firstkey) {
        // move right, looking for the next key
        while (contents_.getDataVal(splitpos) == splitkey) {
          splitpos++;
        }
        splitkey = contents_.getDataVal(splitpos);
      } else {
        // move left, looking for first entry having that ky
        while (contents_.getDataVal(splitpos-1) == splitkey) {
          splitpos--;
        }
      }
      file::BlockId newblk = contents_.split(splitpos, -1);
      DirEntry de(splitkey, newblk.number());
      return de;
    }
  }

  bool BTreeLeaf::tryOverflow() {
    scan::Constant firstkey = contents_.getDataVal(0);
    int flag = contents_.getFlag();
    if (!(searchkey_ == firstkey) || flag < 0) {
      return false;
    }
    contents_.close();
    file::BlockId nextblk(filename_, flag);
    contents_ = BTPage(transaction_, nextblk, layout_);
    currentslot_ = 0;
    return true;
  }

  bool BTreeLeaf::isNull() {
    return contents_.isNull();
  }
}  // namespace indexing
