/* Copyright 2021 Yutaro Yamanaka */
#pragma once

namespace parse {
class Object {
 public:
    virtual ~Object() = default;
    virtual int getID() = 0;
};
}  // namespace parse
