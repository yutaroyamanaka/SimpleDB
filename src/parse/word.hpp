/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>

namespace parse {
class Word {
 public:
    static const char EQUAL_SIGN;
    static const char COMMA;
    static const char LEFT_PARENTHESIS;
    static const char RIGHT_PARENTHESIS;
    static const char QUOTATION;
    static const std::string AMPERSAND;
    static const std::string SELECT;
    static const std::string FROM;
    static const std::string WHERE;
    static const std::string INSERT;
    static const std::string DELETE;
    static const std::string UPDATE;
    static const std::string TABLE;
    static const std::string VIEW;
    static const std::string INDEX;
    static const std::string CREATE;
    static const std::string SET;
    static const std::string AS;
    static const std::string ON;
    static const std::string INTO;
    static const std::string ORDER;
    static const std::string BY;
    static const std::string INT;
    static const std::string VARCHAR;
    static const std::string VALUES;
    static const std::string SPACE;
};
}  // namespace parse
