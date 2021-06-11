#pragma once
#include <string>
#include <vector>
#include <memory>

namespace file {
class Page {
  public:
    Page(int block_size);
    Page(std::shared_ptr<std::vector<char>>& byte_buffer);
    int getInt(int offset) const;
    void setInt(int offset, uint32_t n);
    std::vector<char> getBytes(int offset);
    void setBytes(int offset, std::vector<char>& byte_buffer);
    std::string getString(int offset);
    void setString(int offset, std::string s);
    static int maxLength(int strlen);
    std::shared_ptr<std::vector<char>> contents();
  private:
    std::shared_ptr<std::vector<char>> byte_buffer_;
};
}
