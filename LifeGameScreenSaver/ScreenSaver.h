#pragma once

#include "../LifeGame/LifeGame.h"

class ScreenSaver : public LifeGame {
  using base = LifeGame;
public:
  ~ScreenSaver() override;

  void prepareSettings(Settings* s) override;
  void setup() override;
  void mouseMove(ci::app::MouseEvent event) override;
};
