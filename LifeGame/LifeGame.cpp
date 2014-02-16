#include "stdafx.h"
#include "LifeGame.h"

#include <cinder/cinder.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

using namespace std;

void square10x10(Universe &u, Point offset) {
  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 10; ++x) {
      u.addn({ offset.x + x, offset.y + y }, CellState::ALIVE);
    }
  }
}

void glider(Universe &u, Point offset) {
  u.addn({ offset.x + 1, offset.y + 0 }, CellState::ALIVE);
  u.addn({ offset.x + 2, offset.y + 1 }, CellState::ALIVE);
  u.addn({ offset.x + 0, offset.y + 2 }, CellState::ALIVE);
  u.addn({ offset.x + 1, offset.y + 2 }, CellState::ALIVE);
  u.addn({ offset.x + 2, offset.y + 2 }, CellState::ALIVE);
}

LifeGame::LifeGame()
    : suspend_{ false }, cellSize_(INIT_CELL_SIZE), dragging_(false) {
  auto engine = mt19937{ random_device()() };
  rand_ = [=](int range) mutable {
    auto idist = uniform_int_distribution<int>(0, range);
    return idist(engine);
  };
}

void LifeGame::setup() {
  font_ = TextureFont::create(Font("Consolas", 20));
  glider(universe_, { 0, 0 });
}

void LifeGame::draw() {
  gl::clear();
  for (const auto &cell : universe_) {
    gl::color(color(cell));
    float x = cell.pos().x * cellSize_ + offset_.x;
    float y = cell.pos().y * cellSize_ + offset_.y;
    gl::drawSolidRect({ x, y, x + cellSize_, y + cellSize_ });
  }
  font_->drawString(sysinfo_.msg(), getWindowBounds());
}

void LifeGame::update() {
  if (!suspend_) {
    sysinfo_.onPreGen(universe_);

    universe_.nextGeneration(back_);
    universe_.swap(back_);

    sysinfo_.onPostGen(universe_);
  }
}

Color LifeGame::color(const Cell &cell) const {
  Color yellow{ Color::hex(0xFFFF00) };
  return cell.isDead() ? Color::black() : yellow;
}

void LifeGame::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_RETURN:
    universe_ = bigBang();
    offset_ = {};
    break;

  case KeyEvent::KEY_ESCAPE:
    quit();
    break;

  case KeyEvent::KEY_SPACE:
    suspend_ = !suspend_;
    break;

  case KeyEvent::KEY_c:
    universe_ = {};
    break;

  case KeyEvent::KEY_f:
    setFullScreen(!isFullScreen());
    break;

  case KeyEvent::KEY_HOME:
  case KeyEvent::KEY_r:
    offset_ = {};
    break;
  }
}

void LifeGame::keyDown(KeyEvent e) {
  switch (e.getCode()) {

  case KeyEvent::KEY_UP:
    offset_.y += cellSize_;
    break;

  case KeyEvent::KEY_DOWN:
    offset_.y -= cellSize_;
    break;

  case KeyEvent::KEY_LEFT:
    offset_.x += cellSize_;
    break;

  case KeyEvent::KEY_RIGHT:
    offset_.x -= cellSize_;
    break;
  }
}

Point LifeGame::screenToUniverse(const Vec2i &v) const {
  return {(int)((v.x - offset_.x) / cellSize_),
          (int)((v.y - offset_.y) / cellSize_) };
}

void LifeGame::mouseUp(MouseEvent e) {
  if (e.isLeft())
    dragging_ = false;
}

void LifeGame::mouseDrag(MouseEvent e) {
  if (e.isRightDown()) {
    universe_.add(screenToUniverse(e.getPos()), CellState::ALIVE);
    return;
  }

  if (dragging_) {
    offset_ = e.getPos() - mouseDownOffset_;
  }
}

void LifeGame::mouseDown(MouseEvent e) {
  if (e.isLeft()) {
    if (!dragging_) {
      dragging_ = true;
      mouseDownOffset_ = e.getPos() - offset_;
    }
  } else if (e.isRight()) {
    auto pt = screenToUniverse(e.getPos());
    if (universe_[pt].isDead())
      universe_.add(pt, CellState::ALIVE);
    else
      universe_.add(pt, CellState::DEAD);
  }
}

void LifeGame::mouseWheel(ci::app::MouseEvent e) {
  if (!e.isControlDown()) {
    offset_.y += e.getWheelIncrement() * cellSize_ * SCROLL_LINES;
  } else {
    zoom(e.getWheelIncrement());
  }
}

void LifeGame::zoom(float scale) {
  cellSize_ += scale;
  if (cellSize_ < MIN_CELL_SIZE)
    cellSize_ = MIN_CELL_SIZE;
  if (cellSize_ > MAX_CELL_SIZE)
    cellSize_ = MAX_CELL_SIZE;
}

Universe LifeGame::bigBang() const {
  Universe u;
  int offsetx = (int)(getWindowWidth() / cellSize_ / 3) / 2;
  int offsety = (int)(getWindowHeight() / cellSize_ / 3) / 2;
  int width = (int)(getWindowWidth() / cellSize_ - offsetx * 2);
  int height = (int)(getWindowHeight() / cellSize_ - offsety * 2);
  for (int i = 0; i < width * height * BORN_RATE; ++i) {
    int x = offsetx + rand_(width);
    int y = offsety + rand_(height);
    u.addn({ x, y }, CellState::ALIVE);
  }
  return u;
}

CINDER_APP_BASIC(LifeGame, RendererGl)
