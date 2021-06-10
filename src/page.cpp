#include "page.hpp"
#include <cstring>

Page::Page(int block_size) {
  byte_buffer_ = std::make_shared<std::vector<char>>(block_size);
}

Page::Page(std::shared_ptr<std::vector<char>>& byte_buffer) : byte_buffer_(byte_buffer) {

}

int Page::getInt(int offset) const {
  if(offset + sizeof(int) > (*byte_buffer_).size()) {
    exit(1);
  }

  int num;
  std::memcpy(&num, &(*byte_buffer_)[offset], sizeof(int));
  return num;
}

void Page::setInt(int offset, int n) {
  if(offset + sizeof(int) > (*byte_buffer_).size()) {
    exit(1);
  }
  std::memcpy(&(*byte_buffer_)[offset], &n, sizeof(int));
}

std::vector<char> Page::getBytes(int offset) {
  int len = getInt(offset);

  if(offset + sizeof(int) + len > (*byte_buffer_).size()) {
    exit(1);
  }

  std::vector<char> byte_vector(len);
  memcpy(&byte_vector[0], &(*byte_buffer_)[offset + sizeof(int)], sizeof(int) * len);
  return byte_vector;
}

void Page::setBytes(int offset, std::vector<char>& byte_buffer) {
  int len = byte_buffer.size();
  if(offset + sizeof(int) + len > (*byte_buffer_).size()) {
    exit(1);
  }

  setInt(offset, len);
  memcpy(&(*byte_buffer_)[offset + sizeof(int)], &byte_buffer[0], len);
}

std::string Page::getString(int offset) {
  std::vector<char> bytes = getBytes(offset);
  std::string s(bytes.begin(), bytes.end());
  return s;
}

void Page::setString(int offset, std::string s) {
  std::vector<char> bytes(s.begin(), s.end());
  setBytes(offset, bytes);
}

int Page::maxLength(int strlen) {
  return sizeof(int) + strlen * sizeof(char);
}

std::shared_ptr<std::vector<char>> Page::contents() {
  return byte_buffer_;
}
