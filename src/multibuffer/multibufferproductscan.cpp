/* Copyright 2021 Yutaro Yamanaka */
#include "multibuffer/multibufferproductscan.hpp"

namespace multibuffer {
  MultiBufferProductScan::MultiBufferProductScan(const std::shared_ptr<scan::Scan>& lhsscan, const std::string& filename,
      const record::Layout& layout, tx::Transaction* transaction)
    : transaction_(transaction), lhsscan_(lhsscan), filename_(filename), layout_(layout) {
      filesize_ = transaction_->size(filename_);
      int available = transaction_->availableBuffs();
      chunksize_ = BufferNeeds::bestFactor(available, filesize_);
      beforeFirst();
    }

  void MultiBufferProductScan::beforeFirst() {
    nextblknum_ = 0;
    useNextChunk();
  }

  bool MultiBufferProductScan::next() {
    while (!prodscan_->next()) {
      if (!useNextChunk()) {
        return false;
      }
    }
    return true;
  }

  void MultiBufferProductScan::close() {
    prodscan_->close();
  }

  int MultiBufferProductScan::getInt(const std::string& fldname) {
    return prodscan_->getInt(fldname);
  }

  std::string MultiBufferProductScan::getString(const std::string& fldname) {
    return prodscan_->getString(fldname);
  }

  scan::Constant MultiBufferProductScan::getVal(const std::string& fldname) {
    return prodscan_->getVal(fldname);
  }

  bool MultiBufferProductScan::hasField(const std::string& fldname) {
    return prodscan_->hasField(fldname);
  }

  bool MultiBufferProductScan::useNextChunk() {
    if (rhsscan_) {
      rhsscan_->close();
    }
    if (nextblknum_ >= filesize_) {
      return false;
    }
    int end = nextblknum_ + chunksize_ - 1;
    if (end >= filesize_) {
      end = filesize_ - 1;
    }
    rhsscan_ = std::static_pointer_cast<scan::Scan>(std::make_shared<ChunkScan>(transaction_, filename_, layout_, nextblknum_, end));
    lhsscan_->beforeFirst();
    prodscan_ = std::static_pointer_cast<scan::Scan>(std::make_shared<scan::ProductScan>(lhsscan_, rhsscan_));
    nextblknum_ = end + 1;
    return true;
  }
}  // namespace multibuffer
