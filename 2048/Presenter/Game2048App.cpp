#include "Game2048App.h"

#include "Config.h"
#include "BoardLayout.h"
#include "Cell.h"

#include "../View/VerticalLabel.h"
#include "../View/Animation.h"
#include "../View/Timer.h"

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
  int score_value = 0;
  VerticalLabel score;
  Board board;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  setFrameRate(Config::FRAME_RATE);

  d->score.setBackColor(Color::hex(Config::BOARD_COLOR));
  d->score.setSize({ 100.f, 100.f });
  d->score.setLocation({ BoardLayout::boardRect().x2 - 100.f,
                       BoardLayout::boardRect().y1 - 110.f });
  d->score.setLabelColor(Color::hex(Config::LABEL_FORE_COLOR));
  d->score.setLabel("SCORE");
  d->score.setLabelFont(Font("Arial", 30));
  d->score.setTextColor(Color::hex(Config::SCORE_COLOR));
  d->score.setTextFont(Font("Arial", Config::FONT_WEIGHT));

  d->board = Board(Config::SIZE, Config::SIZE);
  d->board.spawn();
  d->board.spawn();
  d->board.addListener(*this);

  Animation::timer()->reset();
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
  d->score.setText(to_string(d->score_value));

  if (!d->board.moves_available()) {
    d->gameover = true;
  }
}

void Game2048App::draw() {
  gl::clear(Color::hex(Config::BK_COLOR));
  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(getWindowSize());
  d->board.draw(BoardLayout::boardRect());

  if (d->gameover) {
    drawGameOver(BoardLayout::boardRect());
  }

  d->score.draw();

  Animation::timer()->reset();
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

void Game2048App::onPieceMerged(const Piece &from, const Piece &to) {
  d->score_value += (1 << from.value) + (1 << to.value);
}

CINDER_APP_BASIC(Game2048App, RendererGl);
