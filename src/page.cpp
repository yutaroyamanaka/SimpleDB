/* Copyright 2021 Yutaro Yamanaka */
#include "page.hpp"
#include <cstring>
#include <iostream>

namespace file{
Page::Page(int block_size) {
  byte_buffer_ = std::make_shared<std::vector<char>>(block_size);
}

Page::Page(std::shared_ptr<std::vector<char>>& byte_buffer) : byte_buffer_(byte_buffer) {
}

int Page::getInt(int offset) const {
  if(offset + sizeof(uint32_t) > (*byte_buffer_).size()) {
    exit(1);
  }

  uint32_t n;
  memcpy(&n, &(*byte_buffer_)[offset], sizeof(uint32_t));
  return n;
}

void Page::setInt(int offset, uint32_t n) {
  if(offset + sizeof(uint32_t) > (*byte_buffer_).size()) {
    exit(1);
  }
  memcpy(&(*byte_buffer_)[offset], &n, sizeof(uint32_t));
}

std::vector<char> Page::getBytes(int offset) {
  int len = getInt(offset);

  if(offset + sizeof(uint32_t) + len > (*byte_buffer_).size()) {
    exit(1);
  }

  std::vector<char> byte_vector(len);
  memcpy(&byte_vector[0], &(*byte_buffer_)[offset + sizeof(uint32_t)], len);
  return byte_vector;
}

void Page::setBytes(int offset, std::vector<char>& byte_buffer) {
  uint32_t len = byte_buffer.size();
  if(offset + sizeof(uint32_t) + len > (*byte_buffer_).size()) {
    exit(1);
  }

  setInt(offset, len);
  memcpy(&(*byte_buffer_)[offset + sizeof(uint32_t)], &byte_buffer[0], len);
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
  return sizeof(uint32_t) + strlen * sizeof(char);
}

std::shared_ptr<std::vector<char>> Page::contents() {
  return byte_buffer_;
}
}
