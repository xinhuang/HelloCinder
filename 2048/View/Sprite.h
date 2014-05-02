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
  Sprite(const Sprite& sprite);
  ~Sprite();
  Sprite(Animation anim);
  Sprite(std::initializer_list<Layer> layers);

  Sprite& operator=(const Sprite& sprite);

  Animation& layer(int);

  void draw(const ci::Rectf &rect);
};
