#include "blockid.hpp"

namespace file {
BlockId::BlockId() {
}

BlockId::BlockId(const std::string& file_name, int block_num) : file_name_(file_name), block_num_(block_num) {
}

std::string BlockId::fileName() {
  return file_name_;
}

int BlockId::number() {
  return block_num_;
}

bool BlockId::equals(BlockId& bi) {
  return block_num_ == bi.block_num_ && file_name_.compare(bi.file_name_) == 0;
}

std::string BlockId::toString() {
  return "[file " + file_name_ + ", block " + std::to_string(block_num_) + "]";
}

int BlockId::hashCode() {
  return std::hash<std::string>{}(toString());
}
}
