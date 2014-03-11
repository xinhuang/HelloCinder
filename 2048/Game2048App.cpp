#include "Game2048App.h"

#include <cinder/gl/gl.h>

using namespace ci;
using namespace ci::app;

using namespace std;

struct Game2048App::Data {};

Game2048App::Game2048App() : d(make_unique<Data>()) {}

Game2048App::~Game2048App() {}

void Game2048App::draw() {
  gl::clear();

  float width = static_cast<float>(min(getWindowHeight(), getWindowWidth())) - 14;
  Vec2f board_pos{(getWindowWidth() - width) / 2.f,
                  (getWindowHeight() - width) / 2.f, };

  Vec2f board_size{ width, width, };
  drawBoard(board_pos, board_size);
}

void Game2048App::drawBoard(const ci::Vec2f& pos, const ci::Vec2f& size) const {
  gl::drawSolidRoundedRect({ pos, pos + size }, 5.f);
}

CINDER_APP_BASIC(Game2048App, RendererGl);
