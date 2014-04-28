#include "Game2048App.h"

#include "../Model/Piece.h"

#include "Config.h"
#include "Scene.h"
#include "Cell.h"
#include "Board.h"

#include "../Util/Random.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

struct Game2048App::Data {
  vector<unique_ptr<Cell> > cells;
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
  if (offset != Vec2i{} && d->board.moveAll(offset))
    d->board.spawn();
}

void Game2048App::update() {}

void Game2048App::draw() {
  gl::clear(Color::hex(Config::BK_COLOR));

  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(getWindowSize());
  d->board.draw(Scene::boardRect());
}

CINDER_APP_BASIC(Game2048App, RendererGl);
