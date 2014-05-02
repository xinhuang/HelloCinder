#pragma once

#include "IRenderable.h"
#include "TextureRenderer.h"

#include <cinder/gl/Texture.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>

#include <memory>

class Clip {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Clip();
  Clip(const std::shared_ptr<IRenderable> &renderable);
  Clip(ci::gl::TextureRef &tex)
    : Clip(std::shared_ptr<IRenderable>(new TextureRenderer(tex))) {}
  Clip(const Clip &anim);
  ~Clip();

  Clip &operator=(const Clip &anim);

  Clip &moveby(const ci::Vec2f &offset);
  Clip &fadeby(float delta, float from = 1.f);
  Clip &scaleby(float scale, float from = 1.f);
  Clip &duration(float seconds);

  bool finished() const;

  ci::Vec2f offset() const;
  float alpha() const;
  float scale() const;
  float duration() const;

  void draw(float elapsed, ci::Rectf rect);
};
