/* Copyright 2021 Yutaro Yamanaka */
#include "multibuffer/bufferneeds.hpp"

namespace multibuffer {
  int BufferNeeds::bestRoot(int available, int size) {
    int avail = available - 2;  // reserve a couple of buffers
    if (avail <= 1) return 1;
    int k = INT_MAX;
    double i = 1.0;
    while (k > avail) {
      i++;
      k = static_cast<int>(std::ceil(std::pow(size, 1/i)));
    }
    return k;
  }

  int BufferNeeds::bestFactor(int available, int size) {
    int avail = available - 2;  // reserve a couple of buffers
    if (avail <= 1) return 1;
    int k = size;
    double i = 1.0;
    while (k > avail) {
      i++;
      k = static_cast<int>(std::ceil(size / i));
    }
    return k;
  }
}  // namespace multibuffer
