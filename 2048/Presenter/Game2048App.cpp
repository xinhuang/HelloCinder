#include "Game2048App.h"

#include "Config.h"
#include "BoardLayout.h"
#include "Cell.h"

#include "../View/CellRenderer.h"
#include "../View/Gfx.h"
#include "../View/VerticalLabel.h"
#include "../View/GameOverWindow.h"
#include "../View/Animation.h"
#include "../View/Timer.h"
#include "../View/UI.h"

#include "../Util/Random.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

struct Game2048App::Data {
  int score_value = 0;
  shared_ptr<VerticalLabel> score;
  Board board;
  shared_ptr<UI> ui;
  shared_ptr<GameOverWindow> gameOverWindow;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {
  d->ui = make_shared<UI>();
}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  setWindowSize({ 400, 600 });
  gfx()->setup();
  d->ui->setup();
  setFrameRate(Config::FRAME_RATE);

  d->score = d->ui->create<VerticalLabel>();
  d->score->setBackColor(Color::hex(Config::BOARD_COLOR));
  d->score->setSize({ 100.f, 75.f });
  d->score->setLabelHeight(40.f);
  d->score->setLocation({ BoardLayout::boardRect().x2 - 100.f,
                          BoardLayout::boardRect().y1 - 110.f });
  d->score->setLabelColor(Color::hex(Config::LABEL_FORE_COLOR));
  d->score->setLabel("SCORE");
  d->score->setLabelFont(Font("Arial", 30));
  d->score->setTextColor(Color::hex(Config::SCORE_COLOR));
  d->score->setTextFont(Font("Arial", Config::FONT_WEIGHT));

  d->gameOverWindow = d->ui->create<GameOverWindow>();
  d->gameOverWindow->setRect(BoardLayout::boardRect());
  d->gameOverWindow->hide();

  d->board = Board(Config::SIZE, Config::SIZE);
  d->board.spawn();
  d->board.spawn();
  d->board.addListener(*this);
}

void Game2048App::shutdown() {
  gfx()->tearDown();
  d->ui->tearDown();
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

  if (d->gameOverWindow->visible())
    return;

  if (offset != Vec2i{} && d->board.slide(offset))
    d->board.spawn();
}

void Game2048App::update() {
  d->score->setValue(d->score_value);

  if (!d->board.moves_available()) {
    d->gameOverWindow->show();
  }

  d->ui->update();
}

void Game2048App::draw() {
  gl::clear(Color::hex(Config::BK_COLOR));
  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(getWindowSize());
  d->board.draw(BoardLayout::boardRect());

  gfx()->draw();

  d->ui->draw();
}

void Game2048App::resize() { CellRenderer::instance().resize(); }

void Game2048App::onPieceMerged(const Piece &from, const Piece &to) {
  d->score_value += (1 << from.value) + (1 << to.value);
}

CINDER_APP_BASIC(Game2048App, RendererGl);
