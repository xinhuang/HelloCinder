#include "stdafx.h"
#include "LifeGame.h"

#include "Universe.h"
#include "GameConfig.h"

#include <cinder/cinder.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

using namespace std;

#include <ipp.h>

struct LifeGame::Data {
  Data() {
    auto engine = mt19937{ random_device()() };
    rand_ = [=](int range) mutable {
      auto idist = uniform_int_distribution<int>(0, range);
      return idist(engine);
    };
  }

  void createUniverse(int width, int height) {
    now_ = Universe(width, height);
    next_ = Universe(width, height);
  }

  bool suspend_ = false;
  bool dragging_ = false;
  float cellSize_ = GameConfig::INIT_CELL_SIZE;
  Sysinfo sysinfo_;
  ci::gl::TextureFontRef font_;
  std::function<int(int range)> rand_;
  ci::Vec2f offset_;
  ci::Vec2f mouseDownOffset_;

  Universe now_;
  Universe next_;
};

LifeGame::LifeGame() : d(make_unique<Data>()) {}

void LifeGame::setup() {
  d->font_ = TextureFont::create(Font("Consolas", 20));
  d->createUniverse(getWindowWidth(), getWindowHeight());
  ippInit();
}

void LifeGame::draw() {
  gl::draw(d->now_.texture(), getWindowBounds());
  d->font_->drawString(d->sysinfo_.msg(), getWindowBounds());
}

void LifeGame::update() {
  if (!d->suspend_) {
    d->sysinfo_.onPreGen(d->now_);

    d->now_.next(d->next_);
    swap(d->now_, d->next_);

    d->sysinfo_.onPostGen(d->now_);
  }
}

Color LifeGame::color(const Cell &cell) const {
  Color yellow{ Color::hex(0xFFFF00) };
  return cell.isDead() ? Color::black() : yellow;
}

void LifeGame::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_RETURN:
    d->now_ = bigBang();
    d->sysinfo_.init(d->now_);
    d->offset_ = {};
    break;

  case KeyEvent::KEY_ESCAPE:
    quit();
    break;

  case KeyEvent::KEY_SPACE:
    d->suspend_ = !d->suspend_;
    break;

  case KeyEvent::KEY_c:
    d->now_ = {};
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

Point LifeGame::screenToUniverse(const Vec2i &v) const {
  return {(int)((v.x - d->offset_.x) / d->cellSize_),
          (int)((v.y - d->offset_.y) / d->cellSize_) };
}

void LifeGame::mouseUp(MouseEvent e) {
  if (e.isLeft())
    d->dragging_ = false;
}

void LifeGame::mouseDrag(MouseEvent e) {
  if (e.isRightDown()) {
    d->now_.add(screenToUniverse(e.getPos()));
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
    if (!d->now_[pt])
      d->now_.add(pt);
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

Universe LifeGame::bigBang() const {
  int width = getWindowWidth();
  int height = getWindowHeight();
  Universe u(width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = d->rand_(width - 1);
    int y = d->rand_(height - 1);
    u.addn({ x, y });
  }
  return u;
}

CINDER_APP_BASIC(LifeGame, RendererGl)
