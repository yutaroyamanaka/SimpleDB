#pragma once

#include <string>

namespace file {
class BlockId {
  friend bool operator==(const BlockId &Lhs, const BlockId &Rhs);
  friend bool operator!=(const BlockId &Lhs, const BlockId &Rhs);
  friend bool operator<(const BlockId &Lhs, const BlockId &Rhs);
  friend bool operator>(const BlockId &Lhs, const BlockId &Rhs);
  friend bool operator<=(const BlockId &Lhs, const BlockId &Rhs);
  friend bool operator>=(const BlockId &Lhs, const BlockId &Rhs);

  public:
    BlockId();
    BlockId(const std::string& file_name, int block_num);
    std::string fileName();
    int number();
    bool equals(const BlockId& bi);
    bool isNull();
    std::string toString();
    int hashCode();
  private:
    std::string file_name_;
    int block_num_;
};
}
