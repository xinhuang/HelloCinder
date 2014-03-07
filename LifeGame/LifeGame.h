#pragma once

#include <cinder/Cinder.h>
#include <cinder/app/AppBasic.h>
#include <cinder/gl/TextureFont.h>

#include <functional>
#include <deque>
#include <chrono>
#include <memory>

#include "Universe.h"
#include "Sysinfo.h"

namespace GameConfig {
  const float INIT_CELL_SIZE = 5.f;
  const int SCROLL_LINES = 3;
  const float MAX_CELL_SIZE = 20.f;
  const float MIN_CELL_SIZE = 1.f;
  const float BORN_RATE = 0.2f;
}

class LifeGame : public ci::app::AppBasic {
public:
  LifeGame();
  void setup() override;

  void draw() override;
  void update() override;

  void keyUp(ci::app::KeyEvent e) override;
  void keyDown(ci::app::KeyEvent e) override;
  void mouseDrag(ci::app::MouseEvent e) override;
  void mouseDown(ci::app::MouseEvent e) override;
  void mouseUp(ci::app::MouseEvent e) override;
  void mouseWheel(ci::app::MouseEvent e) override;

private:
  Point screenToUniverse(const ci::Vec2i &v) const;
  ci::Color color(const Cell &cell) const;
  Universe bigBang() const;
  void zoom(float scale);

private:
  struct Data;
  std::unique_ptr<Data> d;
};
