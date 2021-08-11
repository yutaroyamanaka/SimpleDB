#include "streamtokenizer.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

void printCurrentToken(std::vector<std::string>& keywords, const parse::StreamTokenizer& tok) {
  if(tok.ttype() == parse::StreamTokenizer::TT_NUMBER) {
    std::cout << "IntConstant " << tok.nval() << std::endl;
  } else if(tok.ttype() == parse::StreamTokenizer::TT_WORD) {
    std::string word = tok.sval();
    auto itr = std::find(keywords.begin(), keywords.end(), word);
    if(itr == keywords.end()) {
      std::cout << "Keyword " << word << std::endl;
    } else {
      std::cout << "Id " << word << std::endl;
    }
  } else if(tok.ttype() == "\'") {
    std::cout << "StringConstant " << tok.sval() << std::endl;
  } else {
    std::cout << "Delimiter " << tok.ttype() << std::endl;
  }
}

TEST(TokenizerTest, Main) {
    std::vector<std::string> keywords = {
      "select",
      "from",
      "where",
      "and",
      "insert",
      "values",
      "delete",
      "update",
      "set",
      "create",
      "table",
      "varchar",
      "int",
      "view",
      "as",
      "index",
      "on"
    };

    std::string query = "SELECT a FROM X, Z WHERE b = 3 AND c = 'foobar'";
    parse::StreamTokenizer tok(query);
    while(tok.getToken() != parse::StreamTokenizer::TT_EOF) {
      printCurrentToken(keywords, tok);
      tok.nextToken();
    }
}
