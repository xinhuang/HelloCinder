#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <map>
#include <random>
#include "ScreenSaver.h"

#include <cinder/cinder.h>

#pragma comment(lib, "Scrnsavw.lib")
#pragma comment(lib, "comctl32.lib")

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

using namespace std;

ScreenSaver::ScreenSaver()
    : suspend_{ false }, cellSize_(INIT_CELL_SIZE), dragging_(false) {
  auto engine = mt19937{ random_device()() };
  rand_ = [=](int range) mutable {
    auto idist = uniform_int_distribution<int>(0, range);
    return idist(engine);
  };
}

void ScreenSaver::setup() {
  font_ = TextureFont::create(Font("Consolas", 20));
  cellSize_ = 2.f;
  universe_ = bigBang();
  sysinfo_.init(universe_);
}

void ScreenSaver::draw() {
  gl::clear();
  for (const auto &cell : universe_) {
    gl::color(color(cell));
    float x = cell.pos().x * cellSize_ + offset_.x;
    float y = cell.pos().y * cellSize_ + offset_.y;
    gl::drawSolidRect({ x, y, x + cellSize_, y + cellSize_ });
  }
  gl::color(Color::white());
  font_->drawString(sysinfo_.msg(), getWindowBounds());
}

void ScreenSaver::update() {
  if (!suspend_) {
    if (sysinfo_.elapsed() >= REFRESH_TIME) {
      universe_ = bigBang();
      sysinfo_.init(universe_);
    }
    sysinfo_.onPreGen(universe_);

    universe_.nextGeneration(back_);
    universe_.swap(back_);

    sysinfo_.onPostGen(universe_);
  }
}

Color ScreenSaver::color(const Cell &cell) const {
  Color yellow{ Color::hex(0xFFFF00) };
  return cell.isDead() ? Color::black() : yellow;
}

Universe ScreenSaver::bigBang() const {
  Universe u;
  int offsetx = 0;
  int offsety = 0;
  int width = (int)(getWindowWidth() / cellSize_ - offsetx * 2);
  int height = (int)(getWindowHeight() / cellSize_ - offsety * 2);
  for (int i = 0; i < width * height * BORN_RATE; ++i) {
    int x = offsetx + rand_(width);
    int y = offsety + rand_(height);
    u.addn({ x, y }, CellState::ALIVE);
  }
  return u;
}

CINDER_APP_SCREENSAVER(ScreenSaver, RendererGl)
