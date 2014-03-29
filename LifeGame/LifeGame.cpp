#include "stdafx.h"
#include "LifeGame.h"

#include "Universe.h"
#include "GameConfig.h"
#include "Random.h"

#include <cinder/cinder.h>
#include <cinder/Font.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

using namespace std;

#include <omp.h>
#include "vax.h"

//#include <tbb/task_scheduler_init.h>

//using namespace tbb;

struct LifeGame::Data {
  bool suspend_ = false;
  bool dragging_ = false;
  float cellSize_ = GameConfig::INIT_CELL_SIZE;
  ci::Font font_;
  ci::Vec2f offset_;
  ci::Vec2f mouseDownOffset_;

  UniverseWrapper<Universe> u_;
  Sysinfo<decltype(u_)> sysinfo_;
};

LifeGame::LifeGame() : d(make_unique<Data>()) {}

void LifeGame::setup() {
  d->font_ = Font("Helvetica", 16);
  d->u_ = bigBang<>(getWindowWidth(), getWindowHeight());
  gl::disableVerticalSync();
  setFrameRate(99999.f);

  ippInit();
  //task_scheduler_init init(4);
  omp_set_num_threads(4);
}

void LifeGame::draw() {
  gl::draw(d->u_.render(), getWindowBounds());

  TextBox label;
  label.setFont(d->font_);
  label.setText(d->sysinfo_.msg(*this));
  gl::draw(gl::Texture(label.render()));
}

void LifeGame::update() {
  if (!d->suspend_) {
    d->sysinfo_.onPreGen(d->u_);

    d->u_.next();

    d->sysinfo_.onPostGen(d->u_);
  }
}

void LifeGame::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_RETURN:
    d->u_ = bigBang<>(getWindowWidth(), getWindowHeight());
    d->sysinfo_.init(d->u_);
    d->offset_ = {};
    break;

  case KeyEvent::KEY_ESCAPE:
    quit();
    break;

  case KeyEvent::KEY_SPACE:
    d->suspend_ = !d->suspend_;
    break;

  case KeyEvent::KEY_c:
    d->u_ = {};
    break;

  case KeyEvent::KEY_f:
    setFullScreen(!isFullScreen());
    break;

  case KeyEvent::KEY_HOME:
  case KeyEvent::KEY_r:
    d->offset_ = {};
    break;
  }
}

void LifeGame::keyDown(KeyEvent e) {
  switch (e.getCode()) {

  case KeyEvent::KEY_UP:
    d->offset_.y += d->cellSize_;
    break;

  case KeyEvent::KEY_DOWN:
    d->offset_.y -= d->cellSize_;
    break;

  case KeyEvent::KEY_LEFT:
    d->offset_.x += d->cellSize_;
    break;

  case KeyEvent::KEY_RIGHT:
    d->offset_.x -= d->cellSize_;
    break;
  }
}

Vec2i LifeGame::screenToUniverse(const Vec2i &v) const {
  return {(int)((v.x - d->offset_.x) / d->cellSize_),
          (int)((v.y - d->offset_.y) / d->cellSize_) };
}

void LifeGame::mouseUp(MouseEvent e) {
  if (e.isLeft())
    d->dragging_ = false;
}

void LifeGame::mouseDrag(MouseEvent e) {
  if (e.isRightDown()) {
    d->u_.add(screenToUniverse(e.getPos()));
    return;
  }

  if (d->dragging_) {
    d->offset_ = e.getPos() - d->mouseDownOffset_;
  }
}

void LifeGame::mouseDown(MouseEvent e) {
  if (e.isLeft()) {
    if (!d->dragging_) {
      d->dragging_ = true;
      d->mouseDownOffset_ = e.getPos() - d->offset_;
    }
  } else if (e.isRight()) {
    auto pt = screenToUniverse(e.getPos());
    d->u_.add(pt);
  }
}

void LifeGame::mouseWheel(ci::app::MouseEvent e) {
  if (!e.isControlDown()) {
    d->offset_.y +=
        e.getWheelIncrement() * d->cellSize_ * GameConfig::SCROLL_LINES;
  } else {
    zoom(e.getWheelIncrement());
  }
}

void LifeGame::zoom(float scale) {
  d->cellSize_ += scale;
  if (d->cellSize_ < GameConfig::MIN_CELL_SIZE)
    d->cellSize_ = GameConfig::MIN_CELL_SIZE;
  if (d->cellSize_ > GameConfig::MAX_CELL_SIZE)
    d->cellSize_ = GameConfig::MAX_CELL_SIZE;
}

CINDER_APP_BASIC(LifeGame, RendererGl)
