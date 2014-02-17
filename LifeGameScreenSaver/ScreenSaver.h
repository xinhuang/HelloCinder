#pragma once

#include <cinder/Cinder.h>
#include <cinder/app/AppScreenSaver.h>
#include <cinder/gl/TextureFont.h>

#include <functional>
#include <deque>
#include <chrono>

#include "../LifeGame/Universe.h"
#include "../LifeGame/Sysinfo.h"

class ScreenSaver : public ci::app::AppScreenSaver {
public:
  ScreenSaver();
  void setup() override;

  void draw() override;
  void update() override;

private:
  ci::Color color(const Cell &cell) const;
  Universe bigBang() const;

private:
  const int SCROLL_LINES = 3;
  const float INIT_CELL_SIZE = 5.f;
  const float MAX_CELL_SIZE = 20.f;
  const float MIN_CELL_SIZE = 1.f;
  const float BORN_RATE = 0.1f;

  bool suspend_;
  bool dragging_;
  float cellSize_;
  Sysinfo sysinfo_;
  ci::gl::TextureFontRef font_;
  std::function<int(int range)> rand_;
  ci::Vec2f offset_;
  ci::Vec2f mouseDownOffset_;
  Universe universe_;
  Universe back_;
};
