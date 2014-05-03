#pragma once

#include "Slice.h"

#include <cinder/gl/Texture.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>

#include <memory>

class Clip {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Clip();
  Clip(const std::shared_ptr<Slice> &renderable);
  Clip(ci::gl::TextureRef &tex)
      : Clip(std::shared_ptr<Slice>(new Slice(tex))) {}
  Clip(const Clip &anim);
  ~Clip();

  Clip &operator=(const Clip &anim);

  Clip &moveby(const ci::Vec2f &offset);
  Clip &fadeby(float delta, float from = 1.f);
  Clip &scale(float from, float to);
  Clip &duration(float seconds);
  Clip &reverse(bool reversed = true);

  bool finished() const;

  ci::Vec2f offset() const;
  float alpha() const;
  float scale() const;
  float duration() const;

  void draw(float elapsed, ci::Rectf rect);
};
