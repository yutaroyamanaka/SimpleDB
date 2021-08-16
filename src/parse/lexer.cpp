/* Copyright 2021 Yutaro Yamanaka */
#include "parse/lexer.hpp"

namespace parse {
  Lexer::Lexer() {
  }

  Lexer::Lexer(const std::string& s) {
    initKeywords();
    tokenizer_ = std::make_unique<StreamTokenizer>(s);
  }

  Lexer &Lexer::operator=(const Lexer& lex) {
    return *this;
  }

  bool Lexer::matchDelm(const char& d) const {
    return d == tokenizer_->ttype()[0];
  }

  bool Lexer::matchIntConstant() const {
    return tokenizer_->ttype() == StreamTokenizer::TT_NUMBER;
  }

  bool Lexer::matchStringConstant() const {
    return Word::QUOTATION == tokenizer_->ttype()[0];
  }

  bool Lexer::matchKeyword(const std::string& w) const {
    return tokenizer_->ttype() == StreamTokenizer::TT_WORD &&
      tokenizer_->sval() == w;
  }

  bool Lexer::matchId() const {
    if (tokenizer_->ttype() != StreamTokenizer::TT_WORD) return false;
    auto itr = std::find(keywords_.begin(), keywords_.end(), tokenizer_->sval());
    return itr == keywords_.end();
  }

  void Lexer::eatDelim(const char& d) {
    if (!matchDelm(d)) throw std::runtime_error("bad syntax exception in eatDelim");
    nextToken();
  }

  int Lexer::eatIntConstant() const {
    if (!matchIntConstant()) throw std::runtime_error("bad syntax exception in EatIntConstant");
    int i = tokenizer_->nval();
    nextToken();
    return i;
  }

  std::string Lexer::eatStringConstant() const {
    if (!matchStringConstant()) throw std::runtime_error("bad syntax exceptionStringConstant");
    std::string s = tokenizer_->sval();
    nextToken();
    return s;
  }

  void Lexer::eatKeyword(const std::string& w) {
    if (!matchKeyword(w)) throw std::runtime_error("bad syntax exception");
    nextToken();
  }

  std::string Lexer::eatId() const {
    if (!matchId()) throw std::runtime_error("bad syntax exception");
    std::string s = tokenizer_->sval();
    nextToken();
    return s;
  }

  void Lexer::nextToken() const {
    tokenizer_->nextToken();
  }

  void Lexer::initKeywords() {
    keywords_ = std::vector<std::string>{
      Word::SELECT,
      Word::FROM,
      Word::WHERE,
      Word::AMPERSAND,
      Word::INSERT,
      Word::VALUES,
      Word::DELETE,
      Word::UPDATE,
      Word::SET,
      Word::CREATE,
      Word::TABLE,
      Word::VARCHAR,
      Word::INT,
      Word::VIEW,
      Word::AS,
      Word::INDEX,
      Word::ON
    };
  }
}  // namespace parse
