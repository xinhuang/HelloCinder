#pragma once

#include "Animation.h"

#include <tuple>
#include <memory>

using Layer = std::pair<const int, Animation>;

class Sprite {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Sprite();
  Sprite(const Sprite &sprite);
  ~Sprite();
  Sprite(Animation anim) : Sprite({ { 0, anim } }) {}
  Sprite(std::initializer_list<Layer> layers);

  Sprite &operator=(const Sprite &sprite);

  int layers() const;
  Animation &layer(int);
  const Animation &layer(int) const;

  void draw(const ci::Rectf &rect);
  void draw();
};
