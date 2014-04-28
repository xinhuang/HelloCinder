#include "Game2048App.h"

#include "Config.h"
#include "Scene.h"
#include "Cell.h"
#include "Board.h"

#include "../View/Label.h"

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
  Label scoreLabel;
  Label score;
  Board board;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  setFrameRate(Config::FRAME_RATE);

  d->scoreLabel.setForeColor(Color::white());
  d->scoreLabel.setBackColor(Color::hex(Config::BOARD_COLOR));
  d->scoreLabel.setText("Score");
  d->scoreLabel.setFontSize(30);
  d->score.setForeColor(Color::white());
  d->score.setBackColor(Color::hex(Config::BOARD_COLOR));

  d->board = Board(Config::SIZE, Config::SIZE);
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
  d->score.setText(to_string(100));

  if (!d->board.moves_available()) {
    d->gameover = true;
  }
}

void Game2048App::draw() {
  gl::clear(Color::hex(Config::BK_COLOR));
  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(getWindowSize());
  d->board.draw(Scene::boardRect());

  if (d->gameover) {
    drawGameOver(Scene::boardRect());
  }

  d->scoreLabel.draw(
      Rectf{ Scene::boardRect().x2 - 100.f, Scene::boardRect().y1 - 50.f - 50.f,
             Scene::boardRect().x2,         Scene::boardRect().y1 - 50.f });

  d->score.draw(
      Rectf{ Scene::boardRect().x2 - 100.f, Scene::boardRect().y1 - 50.f - 20.f,
             Scene::boardRect().x2,         Scene::boardRect().y1 - 20.f });
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
