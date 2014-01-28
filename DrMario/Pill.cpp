#include "stdafx.h"

#include "Pill.h"

const float Pill::Height = 20;
const float Pill::Width = 20;

void Pill::draw() {
  gl::color(lhs_);
  gl::drawSolidRect(leftRect());
  gl::color(rhs_);
  gl::drawSolidRect(rightRect());
}

void Pill::erase(Vec2f loc) {
  if (loc == leftLocation())
    lhs_ = Color();
  else if (loc == rightLocation())
    rhs_ = Color();
  else
    assert(!"Invalid loc found when calling Pill::erase");
}

Rectf Pill::leftRect() const {
  Rectf rect{ location_, location_ + Vec2f{ Height, Width } };
  rect.offset({ -Width, 0 });
  return rect;
}

Rectf Pill::rightRect() const {
  auto left = leftRect();
  left.offset({ Width, 0 });
  return left;
}