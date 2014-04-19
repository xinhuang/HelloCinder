#pragma once

#include <cinder/Rect.h>
#include <cinder/Color.h>
#include <cinder/gl/Texture.h>

class IRenderable {
public:
  virtual ~IRenderable() {}
  virtual void draw(const ci::Rectf &rect) = 0;
};

class TextureRenderer : public IRenderable {
  ci::gl::TextureRef tex_;
  float scale_;

public:
  TextureRenderer(ci::gl::TextureRef &tex, float scale)
      : tex_(tex), scale_(scale) {}

  void draw(const ci::Rectf &rect) final {
    ci::gl::color(ci::Color::white());
    auto size = rect.getSize() * scale_;
    auto offset = (size - rect.getSize()) / 2;
    ci::Vec2f upperLeft = rect.getUpperLeft() - offset;
    ci::gl::enableAlphaBlending();
    ci::gl::draw(tex_, { upperLeft, upperLeft + size });
    ci::gl::disableAlphaBlending();
  }
};
