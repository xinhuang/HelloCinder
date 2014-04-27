#include "Scene.h"

#include "Config.h"

#include <cinder/app/app.h>

using namespace ci;
using namespace ci::app;

using namespace std;

struct Scene::Data {
  float borderSize = 2.f;
};

Scene::Scene() : d(make_unique<Data>()) {}
Scene::~Scene() {}

ci::Vec2f Scene::cellSize() {
  auto borderWidthTotal = (Config::SIZE + 1) * data()->borderSize;
  auto boardHeightTotal = (Config::SIZE + 1) * data()->borderSize;
  return {(boardWidth() - borderWidthTotal) / Config::SIZE,
          (boardHeight() - boardHeightTotal) / Config::SIZE };
}

ci::Vec2f Scene::cellPos(const ci::Vec2i &coord) {
  return boardPos() + Vec2f(data()->borderSize, data()->borderSize) +
         static_cast<Vec2f>(coord) *
             (cellSize() + Vec2f(data()->borderSize, data()->borderSize));
}

ci::Vec2f Scene::distance(const ci::Vec2i &from, const ci::Vec2i &to) {
  return Vec2f(from - to) *
         (cellSize() + Vec2f(data()->borderSize, data()->borderSize));
}

float Scene::boardWidth() {
  return static_cast<float>(min(getWindowWidth(), getWindowHeight())) - 14;
}

float Scene::boardHeight() { return boardWidth(); }

ci::Vec2f Scene::boardPos() {
  return {(getWindowWidth() - boardWidth()) / 2.f,
          (getWindowHeight() - boardHeight()) / 2.f, };
}
