#pragma once

#include "IRenderable.h"

#include <memory>

#include <cinder/gl/Texture.h>
#include <cinder/Rect.h>

class Animation : public IRenderable {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Animation();
  Animation(std::unique_ptr<IRenderable> &&frame);
  Animation(const Animation &other);
  Animation &operator=(const Animation &other);
  ~Animation() override;

  void draw(const ci::Rectf &rect) override;
  void draw(const ci::Rectf &rect, float alpha) final { draw(rect); }

  Animation &reverse();

  friend Animation &operator+=(Animation &anim,
                                std::shared_ptr<IRenderable> &&value);
  template <typename T>
  friend Animation &operator+=(Animation &anim, std::unique_ptr<T> &&value) {
    auto p = std::shared_ptr<IRenderable>(value.release());
    return anim += move(p);
  }
  friend Animation operator+(Animation lhs, const Animation &rhs);
  friend Animation& operator+=(Animation& anim, const Animation &value);
  friend Animation operator*(Animation lhs, const Animation &rhs);
  friend Animation& operator*=(Animation& anim, const Animation &value);
};

Animation scaleBy(ci::gl::TextureRef &tex, float from, float to, int nframe);
Animation moveBy(ci::gl::TextureRef &tex, const ci::Vec2f &offset, int nframe);
Animation fade(ci::gl::TextureRef &tex, float begin, float end, int nframe);

// ---------------------------------------------------------------- //

#include "Timer.h"

class Animation2 {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Animation2();
  Animation2(const std::shared_ptr<IRenderable>& renderable);
  Animation2(const Animation2& anim);
  ~Animation2();

  Animation2& operator=(const Animation2& anim);

  Animation2& moveby(const ci::Vec2f& offset);
  Animation2& fadeby(float delta);
  Animation2& scaleby(float scale);
  Animation2& duration(float seconds);

  ci::Vec2f offset() const;
  float alpha() const;
  float scale() const;

  void draw(ci::Rectf rect);

  static void setTimer(Timer* timer);
  static Timer* timer();
};
