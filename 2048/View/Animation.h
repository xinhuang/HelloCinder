#pragma once

#include "IRenderable.h"
#include "TextureRenderer.h"

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
  friend Animation &operator+=(Animation &anim, const Animation &value);
  friend Animation operator*(Animation lhs, const Animation &rhs);
  friend Animation &operator*=(Animation &anim, const Animation &value);
};

Animation scaleBy(ci::gl::TextureRef &tex, float from, float to, int nframe);
Animation moveBy(ci::gl::TextureRef &tex, const ci::Vec2f &offset, int nframe);
Animation fade(ci::gl::TextureRef &tex, float begin, float end, int nframe);

// ---------------------------------------------------------------- //

#include "Timer.h"

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
  Clip &scaleby(float scale);
  Clip &duration(float seconds);

  void update(float elapsed_seconds);
  bool finished() const;

  ci::Vec2f offset() const;
  float alpha() const;
  float scale() const;
  float duration() const;

  void draw(ci::Rectf rect);
};

// --------------------------------------------------------------- //

#include <initializer_list>

class Animation2 {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Animation2();
  ~Animation2();
  Animation2(const Animation2& anim);
  Animation2(const std::initializer_list<Clip> &clips);

  Animation2& operator=(const Animation2& anim);

  void draw(const ci::Rectf &rect);

  static void setTimer(Timer *timer);
  static Timer *timer();
};

// --------------------------------------------------------------- //

#include <tuple>

using Layer = std::pair<const int, Animation2>;

class Sprite {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Sprite();
  ~Sprite();
  Sprite(Animation anim);
  Sprite(std::initializer_list<Layer> layers);

  Sprite& operator=(const Sprite& sprite);

  void draw(const ci::Rectf &rect);
};
