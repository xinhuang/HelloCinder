#include "BoardLayout.h"

#include "Config.h"

#include <cinder/app/app.h>

using namespace ci;
using namespace ci::app;

using namespace std;

struct BoardLayout::Data {
  int headerHeight = 150;
  float borderSize = 10.f;
};

BoardLayout::BoardLayout() : d(make_unique<Data>()) {}
BoardLayout::~BoardLayout() {}

ci::Vec2f BoardLayout::cellSize() {
  auto borderWidthTotal = (Config::SIZE + 1) * data()->borderSize;
  auto boardHeightTotal = (Config::SIZE + 1) * data()->borderSize;
  return {(boardWidth() - borderWidthTotal) / Config::SIZE,
          (boardHeight() - boardHeightTotal) / Config::SIZE };
}

ci::Vec2f BoardLayout::cellPos(const ci::Vec2i &coord) {
  return boardPos() + Vec2f(data()->borderSize, data()->borderSize) +
         static_cast<Vec2f>(coord) *
             (cellSize() + Vec2f(data()->borderSize, data()->borderSize));
}

ci::Vec2f BoardLayout::distance(const ci::Vec2i &from, const ci::Vec2i &to) {
  return Vec2f(from - to) *
         (cellSize() + Vec2f(data()->borderSize, data()->borderSize));
}

float BoardLayout::boardWidth() {
  return static_cast<float>(
      min(getWindowWidth(), getWindowHeight() - data()->headerHeight) - 14);
}

float BoardLayout::boardHeight() { return boardWidth(); }

ci::Vec2f BoardLayout::boardPos() {
  return {(getWindowWidth() - boardWidth()) / 2.f,
          (getWindowHeight() - boardHeight() + data()->headerHeight) / 2.f, };
}
