#pragma once

#include "Control.h"

namespace animation {
class Gfx;
}

namespace ui {

class UI : public Control {
  static animation::Gfx *_gfx;

public:
  void setup();
  void tearDown();
  void draw() final;

  static animation::Gfx* gfx() { return _gfx; }
};

}
