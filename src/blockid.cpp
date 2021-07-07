#include "blockid.hpp"
#include <string>

namespace file {
bool operator==(const BlockId& Lhs, const BlockId& Rhs) {
  return Lhs.file_name_ == Rhs.file_name_ && Lhs.block_num_ == Rhs.block_num_;
}

bool operator!=(const BlockId& Lhs, const BlockId& Rhs) {
  return Lhs.file_name_ != Rhs.file_name_ || Lhs.block_num_ != Rhs.block_num_;
}

bool operator<(const BlockId& Lhs, const BlockId& Rhs) {
  return Lhs.file_name_ == Rhs.file_name_ ? Lhs.block_num_ < Rhs.block_num_ : Lhs.file_name_ < Rhs.file_name_;
}

bool operator>(const BlockId& Lhs, const BlockId& Rhs) {
  return Lhs.file_name_ == Rhs.file_name_ ? Lhs.block_num_ > Rhs.block_num_ : Lhs.file_name_ > Rhs.file_name_;
}

bool operator<=(const BlockId& Lhs, const BlockId& Rhs) {
  return Lhs > Rhs ? false : true;
}

bool operator>=(const BlockId& Lhs, const BlockId& Rhs) {
  return Lhs < Rhs ? false : true;
}

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

bool BlockId::equals(const BlockId& bi) {
  return block_num_ == bi.block_num_ && file_name_.compare(bi.file_name_) == 0;
}

bool BlockId::isNull() {
  return file_name_.size() == 0;
}

std::string BlockId::toString() {
  return "[file " + file_name_ + ", block " + std::to_string(block_num_) + "]";
}

int BlockId::hashCode() {
  return std::hash<std::string>{}(toString());
}
}
