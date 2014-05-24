#pragma once

#include <cassert>

enum class Anchor {
  NONE = 0x00,
  LEFT = 0x01,
  RIGHT = 0x02,
  TOP = 0x04,
  BOTTOM = 0x08,
  LEFT_RIGHT = LEFT | RIGHT,
  TOP_BOTTOM = TOP | BOTTOM,
  ALL = LEFT_RIGHT | TOP_BOTTOM,
};

namespace {
inline void validate(Anchor anchor) {
  assert(anchor >= Anchor::NONE && anchor <= Anchor::ALL);
}
}

inline Anchor operator|(Anchor lhs, Anchor rhs) {
  validate(lhs);
  validate(rhs);
  auto result = (Anchor)((int)lhs | (int)rhs);
  validate(result);
  return result;
}

inline bool operator&(Anchor lhs, Anchor rhs) {
  validate(lhs);
  validate(rhs);
  return ((int)lhs & (int)rhs) != 0;
}
