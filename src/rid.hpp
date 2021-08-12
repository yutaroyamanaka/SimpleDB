/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>

namespace record {
  class RID {
    friend bool operator==(const RID& lhs, const RID& rhs);
    public:
      RID(int blknum, int slot);
      int blockNumber() const;
      int slot() const;
      bool equals(const RID& rid) const;
      std::string toString() const;
    private:
      int blknum_;
      int slot_;
  };
}
