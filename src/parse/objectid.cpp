/* Copyright 2021 Yutaro Yamanaka */
#include "parse/objectid.hpp"

namespace parse {
  const int ObjectID::INSERT = 0;
  const int ObjectID::REMOVE = 1;
  const int ObjectID::MODIFY = 2;
  const int ObjectID::CREATE_TABLE = 3;
  const int ObjectID::CREATE_VIEW = 4;
  const int ObjectID::CREATE_INDEX = 5;
}  // namespace parse
