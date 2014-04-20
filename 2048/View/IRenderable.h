#pragma once

#include <cinder/Rect.h>
#include <cinder/Color.h>
#include <cinder/gl/Texture.h>

class IRenderable {
public:
  virtual ~IRenderable() {}
  virtual void draw(const ci::Rectf &rect) = 0;
};

class NullRenderer : public IRenderable {
public:
  void draw(const ci::Rectf &rect) final {}
};

class TextureRenderer : public IRenderable {
  ci::gl::TextureRef tex_;
  ci::Vec2f offset_;
  float scale_ = 1.f;

public:
  TextureRenderer(ci::gl::TextureRef &tex, float scale)
      : tex_(tex), scale_(scale) {}
  TextureRenderer(ci::gl::TextureRef &tex, const ci::Vec2f &offset)
      : tex_(tex), offset_(offset) {}

  void draw(const ci::Rectf &rect) final {
    ci::gl::color(ci::Color::white());
    auto size = rect.getSize() * scale_;
    auto offset = offset_ - (size - rect.getSize()) / 2;
    ci::Vec2f upperLeft = rect.getUpperLeft() + offset;
    ci::gl::enableAlphaBlending();
    ci::gl::draw(tex_, { upperLeft, upperLeft + size });
    ci::gl::disableAlphaBlending();
  }
};
