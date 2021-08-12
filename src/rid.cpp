/* Copyright 2021 Yutaro Yamanaka */
#include "rid.hpp"

namespace record {
  bool operator==(const RID& lhs, const RID& rhs) {
    return lhs.blknum_ == rhs.blknum_ && lhs.slot_ == rhs.slot_;
  }

  RID::RID(int blknum, int slot) : blknum_(blknum), slot_(slot) {

  }

  int RID::blockNumber() const {
    return blknum_;
  }

  int RID::slot() const {
    return slot_;
  }

  bool RID::equals(const RID& rid) const {
    return rid.blknum_ == blknum_ && rid.slot_ == slot_;
  }

  std::string RID::toString() const {
    return "[" + std::to_string(blknum_) + ", " + std::to_string(slot_) + "]";
  }
}
