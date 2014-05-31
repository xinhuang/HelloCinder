#pragma once

#include <cinder/gl/Texture.h>
#include <cinder/Vector.h>
#include <cinder/Color.h>

#include <cassert>
#include <stdexcept>

class Slice {
  ci::gl::TextureRef tex_;

public:
  Slice() {}
  Slice(const ci::gl::TextureRef &tex) : tex_(tex) {}
  ~Slice() {
    int a = 0;
  }

  virtual void draw(const ci::Rectf &rect, float alpha = 1.f) {
    if (!tex_) {
      return;
    }

    ci::gl::color(ci::ColorA(1.f, 1.f, 1.f, alpha));
    ci::gl::enableAlphaBlending();
    ci::gl::draw(tex_, rect);
    ci::gl::disableAlphaBlending();
  }
};
