#pragma once

#include "Animation.h"

#include <tuple>
#include <memory>

using Layer = std::pair<const int, Animation>;

class Sprite {
  struct Data;
  std::unique_ptr<Data> d;

  void draw(const ci::Rectf &rect);

public:
  Sprite();
  Sprite(const Sprite &sprite);
  ~Sprite();
  Sprite(Animation anim) : Sprite({ { 0, anim } }) {}
  Sprite(std::initializer_list<Layer> layers);

  Sprite &operator=(const Sprite &sprite);

#ifdef _DEBUG
  void setName(const std::string &name);
#else
  void setName(const std::string &) {}
#endif

  int depth() const;
  void setDepth(int);

  int layers() const;
  Animation &layer(int);
  const Animation &layer(int) const;

  const ci::Rectf& rect() const;
  void setRect(const ci::Rectf &rect);

  void draw();
};
