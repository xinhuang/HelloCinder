#pragma once

#include "IRenderable.h"

#include <cinder/gl/Texture.h>
#include <cinder/Vector.h>
#include <cinder/Color.h>
#include <cassert>

class Slice : public IRenderable {
  ci::gl::TextureRef tex_;

public:
  Slice(ci::gl::TextureRef &tex)
    : tex_(tex){}

  void draw(const ci::Rectf &rect, float alpha) final {
    ci::gl::color(ci::ColorA(1.f, 1.f, 1.f, alpha));
    ci::gl::enableAlphaBlending();
    ci::gl::draw(tex_, rect);
    ci::gl::disableAlphaBlending();
  }
};
