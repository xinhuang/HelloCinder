#include "Scene.h"

#include "Config.h"

#include <cinder/app/app.h>

using namespace ci;
using namespace ci::app;

using namespace std;

struct Scene::Data {
  float borderSize = 0.f;
};

Scene::Scene() : d(make_unique<Data>()) {}
Scene::~Scene() {}

ci::Vec2f Scene::cellSize() {
  return { boardWidth() / Config::SIZE, boardHeight() / Config::SIZE };
}

ci::Vec2f Scene::distance(const ci::Vec2i &from, const ci::Vec2i &to) {
  return Vec2f(from - to) *
         (cellSize() + Vec2f(data()->borderSize, data()->borderSize));
}

const ci::Vec2f Scene::cellPos(const ci::Vec2i &coord) {
  return boardPos() + static_cast<Vec2f>(coord) * cellSize();
}

float Scene::boardWidth() {
  return static_cast<float>(min(getWindowWidth(), getWindowHeight())) - 14;
}

float Scene::boardHeight() { return boardWidth(); }

ci::Vec2f Scene::boardPos() {
  return {(getWindowWidth() - boardWidth()) / 2.f,
          (getWindowHeight() - boardHeight()) / 2.f, };
}
