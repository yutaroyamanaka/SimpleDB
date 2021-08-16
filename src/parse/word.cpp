/* Copyright 2021 Yutaro Yamanaka */
#include "parse/word.hpp"

namespace parse {
  const char Word::EQUAL_SIGN = '=';
  const char Word::COMMA = ',';
  const char Word::LEFT_PARENTHESIS = '(';
  const char Word::RIGHT_PARENTHESIS = ')';
  const char Word::QUOTATION = '\'';
  const std::string Word::AMPERSAND = "&";
  const std::string Word::SELECT = "select";
  const std::string Word::FROM = "from";
  const std::string Word::WHERE = "where";
  const std::string Word::INSERT = "insert";
  const std::string Word::DELETE = "delete";
  const std::string Word::UPDATE = "update";
  const std::string Word::TABLE = "table";
  const std::string Word::VIEW = "view";
  const std::string Word::INDEX = "index";
  const std::string Word::CREATE = "create";
  const std::string Word::SET = "set";
  const std::string Word::AS = "as";
  const std::string Word::ON = "on";
  const std::string Word::INTO = "into";
  const std::string Word::INT = "int";
  const std::string Word::VARCHAR = "varchar";
  const std::string Word::VALUES = "values";
}  // namespace parse
