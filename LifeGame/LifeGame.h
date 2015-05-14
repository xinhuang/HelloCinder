#pragma once

#include "Sysinfo.h"

#include <cinder/app/AppBasic.h>

class LifeGame : public ci::app::AppBasic {
public:
  LifeGame();
  void setup() override;

  void draw() override;
  void update() override;

  void resize() final;
  void keyUp(ci::app::KeyEvent e) final;
  
private:
  void createUniverse(int width, int height);

private:
  struct Data;
  std::unique_ptr<Data> d;
};
