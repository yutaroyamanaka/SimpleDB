#include "streamtokenizer.hpp"
#include "lexer.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

TEST(LexerTest, Main) {
  std::vector<std::string> lists = {
    "A = 1",
    "3 = B"
  };

  for(const auto& str: lists) {
    parse::Lexer lex(str);
    std::string x = "";
    int y = 0;
    if(lex.matchId()) {
      x = lex.eatId(); 
      lex.eatDelim('=');
      y = lex.eatIntConstant();
    } else {
      y = lex.eatIntConstant();
      lex.eatDelim('=');
      x = lex.eatId();
    }
    std::cout << x << " equals " << y << std::endl;
  }
}
