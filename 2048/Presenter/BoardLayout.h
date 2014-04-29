#pragma once

#include <memory>

#include <cinder/Vector.h>
#include <cinder/Rect.h>

class BoardLayout {
  struct Data;
  std::unique_ptr<Data> d;

  BoardLayout();

  static BoardLayout &instance() {
    static BoardLayout env;
    return env;
  }
  static std::unique_ptr<Data> &data() { return instance().d; }

public:
  ~BoardLayout();

  static ci::Vec2f cellSize();
  static ci::Vec2f cellPos(const ci::Vec2i &coord);
  static ci::Rectf cellRect(const ci::Vec2i &coord) {
    auto pos = cellPos(coord);
    return { pos, pos + cellSize() };
  }

  static ci::Vec2f distance(const ci::Vec2i &from, const ci::Vec2i &to);

  static float boardWidth();
  static float boardHeight();
  static ci::Vec2f boardPos();
  static ci::Vec2f boardSize() {
    return { boardWidth(), boardHeight() };
  }
  static ci::Rectf boardRect() {
    auto pos = boardPos();
    return { pos, pos + boardSize() };
  };
};
