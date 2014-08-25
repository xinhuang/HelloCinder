#pragma once

#include "Clip.h"

#include <memory>
#include <initializer_list>

#include <cinder/Rect.h>

class Timer;

namespace animation {

enum class WrapMode {
  ONCE,
  CLAMP_FOREVER,
  LOOP,
};

class Animation {
  struct Data;
  std::unique_ptr<Data> d;

  void wrap();

public:
  Animation();
  ~Animation();
  Animation(const Animation &anim);
  Animation(const std::initializer_list<Clip> &clips);

  Animation &operator=(const Animation &anim);

  Animation &wrap(WrapMode mode);
  void draw(const ci::Rectf &rect);

  bool isPlaying() const;
  float duration() const;

  friend Animation operator+(Animation lhs, const Animation &rhs);
};

}
