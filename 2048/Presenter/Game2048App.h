#pragma once

#include <cinder/app/AppBasic.h>

#include <memory>
#include <vector>

class Piece;
class Cell;

class Game2048App : public ci::app::AppBasic {
  struct Data;
  std::unique_ptr<Data> d;

  void drawGameOver(const ci::Rectf& rect) const;
public:
  Game2048App();
  ~Game2048App();

  void setup() final;

  void draw() final;
  void update() final;
  void keyUp(ci::app::KeyEvent e) final;
};
