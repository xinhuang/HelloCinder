#include "BoardLayout.h"

#include "Config.h"

#include <cinder/app/app.h>

using namespace ci;
using namespace ci::app;

using namespace std;

struct BoardLayout::Data {};

BoardLayout::BoardLayout() : d(make_unique<Data>()) {}
BoardLayout::~BoardLayout() {}

ci::Vec2f BoardLayout::cellSize() {
  auto borderWidthTotal = (Config::SIZE + 1) * Config::BORDER_WIDTH;
  auto boardHeightTotal = (Config::SIZE + 1) * Config::BORDER_WIDTH;
  return {(boardWidth() - borderWidthTotal) / Config::SIZE,
          (boardHeight() - boardHeightTotal) / Config::SIZE };
}

ci::Vec2f BoardLayout::cellPos(const ci::Vec2i &coord) {
  return boardPos() + Vec2f(Config::BORDER_WIDTH, Config::BORDER_WIDTH) +
         static_cast<Vec2f>(coord) *
             (cellSize() + Vec2f(Config::BORDER_WIDTH, Config::BORDER_WIDTH));
}

ci::Vec2f BoardLayout::distance(const ci::Vec2i &from, const ci::Vec2i &to) {
  return Vec2f(from - to) *
         (cellSize() + Vec2f(Config::BORDER_WIDTH, Config::BORDER_WIDTH));
}

float BoardLayout::boardWidth() {
  return static_cast<float>(
      min(getWindowWidth(), getWindowHeight() - (int)Config::HEADER_HEIGHT) -
      14);
}

float BoardLayout::boardHeight() { return boardWidth(); }

ci::Vec2f BoardLayout::boardPos() {
  return {(getWindowWidth() - boardWidth()) / 2.f, Config::HEADER_HEIGHT };
}
