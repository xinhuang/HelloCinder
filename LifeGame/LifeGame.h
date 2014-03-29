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
  void createUniverse(int width, int height);
  ci::Vec2i screenToUniverse(const ci::Vec2i &v) const;
  void zoom(float scale);

private:
  struct Data;
  std::unique_ptr<Data> d;
};
