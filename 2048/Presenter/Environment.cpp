#include "Environment.h"

#include "Config.h"

#include <cinder/app/app.h>

using namespace ci;
using namespace ci::app;

using namespace std;

struct Environment::Data {
  float borderSize = 0.f;
};

Environment::Environment() : d(make_unique<Data>()) {}
Environment::~Environment() {}

ci::Vec2f Environment::cellSize() const {
  return { boardWidth() / Config::SIZE, boardHeight() / Config::SIZE };
}

ci::Vec2f Environment::distance(const ci::Vec2i& from, const ci::Vec2i& to) const {
  return Vec2f(from - to) * (cellSize() + Vec2f(d->borderSize, d->borderSize));
}

const ci::Vec2f Environment::cellPos(const ci::Vec2i& coord) const {
  return boardPos() + static_cast<Vec2f>(coord)* cellSize();
}

float Environment::boardWidth() const {
  return static_cast<float>(min(getWindowWidth(), getWindowHeight())) - 14;
}

float Environment::boardHeight() const {
  return boardWidth();
}

ci::Vec2f Environment::boardPos() const {
  return {(getWindowWidth() - boardWidth()) / 2.f,
          (getWindowHeight() - boardHeight()) / 2.f, };
}
