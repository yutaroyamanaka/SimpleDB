#pragma once

#include <string>

class BlockId {
  public:
    BlockId(std::string file_name, int block_num);
    std::string fileName();
    int number();
    bool equals(BlockId& bi);
    std::string toString();
    int hashCode();
  private:
    std::string file_name_;
    int block_num_;
};
