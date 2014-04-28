#include "Game2048App.h"

#include "Config.h"
#include "Scene.h"
#include "Cell.h"
#include "Board.h"

#include "../Util/Random.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

struct Game2048App::Data {
  bool gameover = false;
  Board board;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  d->board = Board(Config::SIZE, Config::SIZE);
  setFrameRate(Config::FRAME_RATE);
  d->board.spawn();
  d->board.spawn();
}

void Game2048App::keyUp(ci::app::KeyEvent e) {
  Vec2i offset;
  switch (e.getCode()) {
  case KeyEvent::KEY_LEFT:
    offset = { -1, 0 };
    break;

  case KeyEvent::KEY_RIGHT:
    offset = { 1, 0 };
    break;

  case KeyEvent::KEY_UP:
    offset = { 0, -1 };
    break;

  case KeyEvent::KEY_DOWN:
    offset = { 0, 1 };
    break;

  case KeyEvent::KEY_ESCAPE:
    quit();
    return;

  default:
    return;
  }

  if (d->gameover)
    return;

  if (offset != Vec2i{} && d->board.slide(offset))
    d->board.spawn();
}

void Game2048App::update() {
  if (d->board.moves_available())
    return;
  d->gameover = true;
}

void Game2048App::draw() {
  gl::clear(Color::hex(Config::BK_COLOR));
  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(getWindowSize());
  d->board.draw(Scene::boardRect());

  if (d->gameover) {
    drawGameOver(Scene::boardRect());
  }
}

void Game2048App::drawGameOver(const Rectf &rect) const {
  gl::enableAlphaBlending();

  gl::color(ColorA(1.f, 1.f, 1.f, 0.6f));
  gl::drawSolidRect(rect);

  gl::color(Color::white());
  auto font = Font("Arial", 100.f);
  auto tb = TextBox().alignment(TextBox::CENTER).font(font).text("Game over!");
  tb.setSize(rect.getSize());
  tb.setColor(Color::hex(0x776E65));
  gl::draw(tb.render(), rect);

  gl::disableAlphaBlending();
}

CINDER_APP_BASIC(Game2048App, RendererGl);
