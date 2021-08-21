/* Copyright 2021 Yutaro Yamanaka */
#pragma once

namespace parse {
class ObjectID {
 public:
    static const int INSERT;
    static const int REMOVE;
    static const int MODIFY;
    static const int CREATE_TABLE;
    static const int CREATE_VIEW;
    static const int CREATE_INDEX;
};
}  // namespace parse
