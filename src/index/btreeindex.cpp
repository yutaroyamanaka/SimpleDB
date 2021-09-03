/* Copyright 2021 Yutaro Yamanaka */
#include "index/btreeindex.hpp"

namespace index {
  BTreeIndex::BTreeIndex(tx::Transaction* transaction, const std::string& idxname, const record::Layout& leafLayout)
    : transaction_(transaction), leafLayout_(leafLayout) {
      // deal with the leaves
      leaftbl_ = idxname + "leaf";
      if (transaction_->size(leaftbl_) == 0) {
        file::BlockId blk = transaction_->append(leaftbl_);
        BTPage node(transaction_, blk, leafLayout_);
        node.format(blk, -1);
      }
      // deal with the directory
      record::Schema dirsch;
      dirsch.add("block", leafLayout_.schema());
      dirsch.add("dataval", leafLayout_.schema());
      std::string dirtbl = idxname + "dir";
      dirLayout_ = record::Layout(dirsch);
      rootblk_ = file::BlockId(dirtbl, 0);
      if (transaction_->size(dirtbl) == 0) {
        // create new root block
        transaction_->append(dirtbl);
        BTPage node(transaction_, rootblk_, dirLayout_);
        node.format(rootblk_, 0);
        // insert initial directory entry
        int fldtype = dirsch.type("dataval");
        scan::Constant minval = (fldtype == record::Schema::INTEGER) ? scan::Constant(INT_MIN) : scan::Constant("");
        node.insertDir(0, minval, 0);
        node.close();
      }
    }

  void BTreeIndex::beforeFirst(const scan::Constant& searchkey) {
    close();
    BTreeDir root(transaction_, rootblk_, dirLayout_);
    int blknum = root.search(searchkey);
    root.close();
    file::BlockId leafblk(leaftbl_, blknum);
    leaf_ = BTreeLeaf(transaction_, leafblk, leafLayout_, searchkey);
  }

  bool BTreeIndex::next() {
    return leaf_.next();
  }

  record::RID BTreeIndex::getDataRid() {
    return leaf_.getDataRid();
  }

  void BTreeIndex::insert(const scan::Constant& dataval, const record::RID& datarid) {
    beforeFirst(dataval);
    DirEntry e = leaf_.insert(datarid);
    leaf_.close();
    if (e.isNull()) {
      return;
    }
    BTreeDir root(transaction_, rootblk_, dirLayout_);
    DirEntry e2 = root.insert(e);
    if (!e2.isNull()) {
      root.makeNewRoot(e2);
    }
    root.close();
  }

  void BTreeIndex::remove(const scan::Constant& dataval, const record::RID& datarid) {
    beforeFirst(dataval);
    leaf_.remove(datarid);
    leaf_.close();
  }

  void BTreeIndex::close() {
    if (!leaf_.isNull()) {
      leaf_.close();
    }
  }

  int BTreeIndex::searchCost(int numblocks, int rgb) {
    return 1 + std::round(log(numblocks) / log(rgb));
  }
}  // namespace index
