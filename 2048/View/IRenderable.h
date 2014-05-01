#pragma once

#include <cinder/Rect.h>

class IRenderable {
public:
  virtual ~IRenderable() {}
  virtual void draw(const ci::Rectf &rect) {
    draw(rect, 1.f);
  };
  virtual void draw(const ci::Rectf &rect, float alpha) = 0;
};
