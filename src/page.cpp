#include "page.hpp"

Page::Page(int block_size) {
  byte_buffer_ = std::make_shared<std::vector<char>>(block_size);
}

Page::Page(std::shared_ptr<std::vector<char>>& byte_buffer) : byte_buffer_(byte_buffer) {

}


