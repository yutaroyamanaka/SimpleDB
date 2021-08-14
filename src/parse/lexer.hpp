/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "parse/streamtokenizer.hpp"

namespace parse {
class Lexer {
 public:
    Lexer();
    Lexer(const std::string& s);
    bool matchDelm(const char& d) const;
    bool matchIntConstant() const;
    bool matchStringConstant() const;
    bool matchKeyword(const std::string& w) const;
    bool matchId() const;
    void eatDelim(const char& d);
    int eatIntConstant() const;
    std::string eatStringConstant() const;
    void eatKeyword(const std::string& w);
    std::string eatId() const;
 private:
    void nextToken() const;
    void initKeywords();
    std::vector<std::string> keywords_;
    std::unique_ptr<StreamTokenizer> tokenizer_;
};
}  // namespace parse
