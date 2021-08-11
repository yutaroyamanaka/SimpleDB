#pragma once
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <memory>
#include <ctype.h>

namespace parse {
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  class StreamTokenizer {
    public:
      StreamTokenizer();
      StreamTokenizer(const std::string& s);
      std::string ttype() const;
      double nval() const;
      std::string sval() const;
      void nextToken();
      std::string getToken() const;
      static const std::string TT_EOF;
      static const std::string TT_WORD;
      static const std::string TT_NUMBER;
      static const char QUOTATION;
   private:
      std::unique_ptr<tokenizer> tokenizer_;
      tokenizer::iterator itr_;
  };
}
