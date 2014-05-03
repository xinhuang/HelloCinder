#pragma once

#include <cinder/gl/Texture.h>
#include <cinder/Vector.h>
#include <cinder/Color.h>

#include <cassert>

class Slice {
  ci::gl::TextureRef tex_;

public:
  Slice() {}
  Slice(ci::gl::TextureRef &tex) : tex_(tex) {}

  virtual void draw(const ci::Rectf &rect, float alpha = 1.f) {
    ci::gl::color(ci::ColorA(1.f, 1.f, 1.f, alpha));
    ci::gl::enableAlphaBlending();
    ci::gl::draw(tex_, rect);
    ci::gl::disableAlphaBlending();
  }
};
