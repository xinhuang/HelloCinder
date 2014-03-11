#pragma once

#include <cinder/app/AppBasic.h>

#include <memory>

class Game2048App : public ci::app::AppBasic {
public:
  Game2048App();
  ~Game2048App();

  void draw() final;

private:
  void drawBoard(const ci::Vec2f& pos, const ci::Vec2f& size) const;

private:
  struct Data;
  std::unique_ptr<Data> d;
};
