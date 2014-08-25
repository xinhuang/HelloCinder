#include "UI.h"

using namespace std;

#include "Gfx.h"

namespace ui {

animation::Gfx *UI::_gfx = nullptr;

void UI::setup() {
  _gfx = new animation::Gfx();
  _gfx->setup();
}

void UI::tearDown() {
  _gfx->tearDown();
  delete _gfx;
  _gfx = nullptr;
}

void UI::draw() {
  Control::draw();
  _gfx->draw();
}
}
