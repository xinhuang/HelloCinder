#pragma once

#include <cinder/Cinder.h>
#include <cinder/app/AppBasic.h>
#include <cinder/gl/TextureFont.h>

#include <functional>
#include <deque>
#include <chrono>

#include "Universe.h"
#include "Sysinfo.h"

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
  const int SCROLL_LINES = 3;
  const float INIT_CELL_SIZE = 5.f;
  const float MAX_CELL_SIZE = 20.f;
  const float MIN_CELL_SIZE = 1.f;
  const float BORN_RATE = 0.2f;

  bool suspend_;
  bool dragging_;
  float cellSize_;
  Sysinfo sysinfo_;
  ci::gl::TextureFontRef font_;
  std::function<int(int range)> rand_;
  ci::Vec2f offset_;
  ci::Vec2f mouseDownOffset_;
  Universe universe_;
};
