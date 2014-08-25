#pragma once

#include "../2048/View/Slice.h"

#include <gmock/gmock.h>

struct MockSlice : public animation::Slice {
  MOCK_METHOD2(draw, void(const ci::Rectf &, float alpha));
};

namespace cinder {

template <typename T>
inline bool operator==(const cinder::RectT<T> &lhs,
                       const cinder::RectT<T> &rhs) {
  return lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2 && lhs.y1 == rhs.y1 &&
         lhs.y2 == rhs.y2;
}
template <typename T>
bool operator!=(const cinder::RectT<T> &lhs, const cinder::RectT<T> &rhs) {
  return !(lhs == rhs);
}
}
