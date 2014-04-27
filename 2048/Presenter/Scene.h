#pragma once

#include <memory>

#include <cinder/Vector.h>
#include <cinder/Rect.h>

class Scene {
  struct Data;
  std::unique_ptr<Data> d;

  Scene();

  static Scene &instance() {
    static Scene env;
    return env;
  }
  static std::unique_ptr<Data>& data() { return instance().d; }

public:
  ~Scene();

  static ci::Vec2f cellSize();
  static const ci::Vec2f cellPos(const ci::Vec2i &coord);

  static ci::Vec2f distance(const ci::Vec2i &from, const ci::Vec2i &to);

  static float boardWidth();
  static float boardHeight();
  static ci::Vec2f boardPos();
  static ci::Vec2f boardSize() {
    return { boardWidth(), boardHeight() };
  }
  static ci::Rectf boardRect() {
    return { boardPos(), boardSize() };
  };
};
