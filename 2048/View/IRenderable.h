#pragma once

#include <cinder/Rect.h>
#include <cinder/Color.h>
#include <cinder/gl/Texture.h>

#include <cassert>

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
  float alpha_ = 1.f;
  float scale_ = 1.f;
  ci::Vec2f offset_;

public:
  TextureRenderer(ci::gl::TextureRef &tex, float scale, float alpha,
                  const ci::Vec2f &offset)
      : tex_(tex), scale_(scale), alpha_(alpha), offset_(offset) {
    assert(alpha_ >= 0.f && alpha_ <= 1.f);
  }

  TextureRenderer(ci::gl::TextureRef &tex, float scale = 1.f)
      : tex_(tex), scale_(scale) {}
  TextureRenderer(ci::gl::TextureRef &tex, const ci::Vec2f &offset)
      : tex_(tex), offset_(offset) {}

  static std::shared_ptr<TextureRenderer> fade(ci::gl::TextureRef &tex,
                                               float alpha) {
    return std::make_shared<TextureRenderer>(tex, 1.f, alpha,
                                             ci::Vec2f{ 0.f, 0.f });
  }

  void draw(const ci::Rectf &rect) final {
    ci::gl::color(ci::ColorA(1.f, 1.f, 1.f, alpha_));
    auto size = rect.getSize() * scale_;
    auto offset = offset_ - (size - rect.getSize()) / 2;
    ci::Vec2f upperLeft = rect.getUpperLeft() + offset;
    ci::gl::enableAlphaBlending();
    ci::gl::draw(tex_, { upperLeft, upperLeft + size });
    ci::gl::disableAlphaBlending();
  }
};
