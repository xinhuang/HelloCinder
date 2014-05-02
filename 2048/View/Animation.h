#pragma once

#include "Clip.h"

#include <memory>
#include <initializer_list>

#include <cinder/Rect.h>

class Timer;

class Animation {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Animation();
  ~Animation();
  Animation(const Animation& anim);
  Animation(const std::initializer_list<Clip> &clips);

  Animation& operator=(const Animation& anim);

  Animation& cyclic(bool value = true);
  void draw(const ci::Rectf &rect);

  float duration() const;

  static void setTimer(Timer *timer);
  static Timer *timer();

  friend Animation operator+(Animation lhs, const Animation &rhs);
};
