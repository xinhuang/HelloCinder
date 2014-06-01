#pragma once

#include "Control.h"

class GameOverWindow : public Control {
  struct Data;
  std::unique_ptr<Data> d;

public:
  GameOverWindow();
  ~GameOverWindow() final;

  void draw() final;
};