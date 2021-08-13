/* Copyright 2021 Yutaro Yamanaka */
#include "streamtokenizer.hpp"

namespace parse {
  const std::string StreamTokenizer::TT_EOF = "EOF";
  const std::string StreamTokenizer::TT_WORD = "WORD";
  const std::string StreamTokenizer::TT_NUMBER = "NUMBER";
  const char StreamTokenizer::QUOTATION = '\'';

  StreamTokenizer::StreamTokenizer() {
  }

  StreamTokenizer::StreamTokenizer(const std::string& s) {
    boost::char_separator<char> sep{" ", ","};
    tokenizer_ = std::make_unique<tokenizer>(s, sep);
    itr_ = tokenizer_->begin();
  }

  std::string StreamTokenizer::ttype() const {
    if (itr_ == tokenizer_->end()) return TT_EOF;

    bool isNum = true;
    bool isAlNum = true;
    std::string str = *itr_;
    for (int i = 0; i < str.size(); i++) {
      if (!isdigit(str[i])) {
        isNum = false;
      }

      if (!isalnum(str[i])) {
        isAlNum = false;
      }
    }

    if (isNum) return TT_NUMBER;
    if (isAlNum) return TT_WORD;
    if (str[0] == QUOTATION) {
      std::string q(1, QUOTATION);
      return q;
    }
    return boost::algorithm::to_lower_copy(*itr_);
  }

  std::string StreamTokenizer::getToken() const {
    if (itr_ == tokenizer_->end()) return TT_EOF;
    return boost::algorithm::to_lower_copy(*itr_);
  }

  double StreamTokenizer::nval() const {
    try {
      return std::stod(*itr_);
    } catch(std::exception& e) {
      throw std::runtime_error("this string is not an integer");
    }
  }

  std::string StreamTokenizer::sval() const {
    return boost::algorithm::to_lower_copy(*itr_);
  }

  void StreamTokenizer::nextToken() {
    if (itr_ != tokenizer_->end()) itr_++;
  }
}  // namespace parse
