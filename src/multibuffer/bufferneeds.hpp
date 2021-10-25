/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <climits>
#include <cmath>

namespace multibuffer {
class BufferNeeds {
 public:
    static int bestRoot(int available, int size);
    static int bestFactor(int available, int size);
};
}  // namespace multibuffer
