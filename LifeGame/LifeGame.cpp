#include "stdafx.h"
#include "LifeGame.h"

#include "Universe.h"
#include "GpuGlslUniverse.h"
#include "GameConfig.h"
#include "Random.h"

#include <cinder/cinder.h>
#include <cinder/Font.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

#include <vector>

using namespace std;

#include <omp.h>
#include "vax.h"

#if defined USE_TBB
#include <tbb/tbb.h>
using namespace tbb;
#endif

struct LifeGame::Data {
  bool suspend_ = false;
  bool dragging_ = false;
  float cellSize_ = GameConfig::INIT_CELL_SIZE;
  ci::Font font_;
  ci::Vec2f offset_;
  ci::Vec2f mouseDownOffset_;
  ci::Area windowBounds_;
  ci::Vec2i windowSize_;

  int iuniverse_ = 0;
  vector<function<unique_ptr<IUniverse>(int, int)>> creators_;
  unique_ptr<IUniverse> u_;
  Sysinfo sysinfo_;
};

LifeGame::LifeGame() : d(make_unique<Data>()) {
  d->creators_.push_back(bigBang<GpuGlslUniverse>);
  d->creators_.push_back(bigBang<CpuLoopUniverse>);
  d->creators_.push_back(bigBang<CpuLoopOmpUniverse>);
  d->creators_.push_back(bigBang<CpuIppUniverse>);
  d->creators_.push_back(bigBang<CpuIppOmpUniverse>);
#if defined USE_TBB
  d->creators_.push_back(bigBang<CpuIppTbbUniverse>);
#endif // USE_TBB
}

void LifeGame::setup() {
  d->font_ = Font("Helvetica", 16);
  gl::disableVerticalSync();
  setFrameRate(99999.f);

  createUniverse(getWindowWidth(), getWindowHeight());

  ippInit();
}

void LifeGame::createUniverse(int width, int height) {
  d->u_ = d->creators_[d->iuniverse_](width, height);
  d->sysinfo_.init(*(d->u_));
  d->offset_ = {};
}

void LifeGame::draw() {
  auto tex = d->u_->render();
  gl::setViewport(d->windowBounds_);
  gl::setMatricesWindow(d->windowSize_);
  gl::draw(tex, d->windowBounds_);

  TextBox label;
  label.setFont(d->font_);
  label.setText(d->sysinfo_.msg(*this));
  gl::draw(gl::Texture(label.render()));
}

void LifeGame::update() {
  if (!d->suspend_) {
    d->sysinfo_.onPreGen(*(d->u_));
    d->u_->next();
    d->sysinfo_.onPostGen(*(d->u_));
  }
}

void LifeGame::resize() {
  d->windowBounds_ = getWindowBounds();
  d->windowSize_ = getWindowSize();
}

void LifeGame::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_RETURN:
    createUniverse(getWindowWidth(), getWindowHeight());
    break;

  case KeyEvent::KEY_RIGHT:
    d->iuniverse_ = (d->iuniverse_ + 1) % d->creators_.size();
    createUniverse(getWindowWidth(), getWindowHeight());
    break;

  case KeyEvent::KEY_LEFT:
    d->iuniverse_ =
        (d->iuniverse_ + d->creators_.size() - 1) % d->creators_.size();
    createUniverse(getWindowWidth(), getWindowHeight());
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
          (int)((v.y - d->offset_.y) / d->cellSize_)};
}

void LifeGame::mouseUp(MouseEvent e) {
  if (e.isLeft())
    d->dragging_ = false;
}

void LifeGame::mouseDrag(MouseEvent e) {
  if (e.isRightDown()) {
    d->u_->add(screenToUniverse(e.getPos()));
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
    d->u_->add(pt);
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

CINDER_APP_BASIC(LifeGame, RendererGl(RendererGl::AA_NONE))
