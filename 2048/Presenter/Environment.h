#pragma once

#include <memory>

#include <cinder/Vector.h>
#include <cinder/Rect.h>

class Environment {
  struct Data;
  std::unique_ptr<Data> d;

  Environment();

public:
  ~Environment();

  static Environment &instance() {
    static Environment env;
    return env;
  }

  ci::Vec2f cellSize() const;
  const ci::Vec2f cellPos(const ci::Vec2i &coord) const;

  ci::Vec2f distance(const ci::Vec2i &from, const ci::Vec2i &to) const;

  float boardWidth() const;
  float boardHeight() const;

  ci::Vec2f boardPos() const;
  ci::Vec2f boardSize() const {
    return { boardWidth(), boardHeight() };
  }
  ci::Rectf boardRect() const {
    return { boardPos(), boardSize() };
  };
};
