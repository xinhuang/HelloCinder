#include "Game2048App.h"

#include "Random.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

struct Piece {
  int value;
  Vec2i pos;
};

struct Game2048App::Data {
  vector<Piece> pieces;
  Font font;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  d->font = Font("Arial", 60);
  spawn();
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
  default:
    return;
  }
  if (moveAll(offset))
    spawn();
}

void Game2048App::update() {}

void Game2048App::draw() {
  gl::clear();

  float width =
      static_cast<float>(min(getWindowHeight(), getWindowWidth())) - 14;
  Vec2f boardPos{(getWindowWidth() - width) / 2.f,
                 (getWindowHeight() - width) / 2.f, };

  Vec2f boardSize{ width, width, };
  drawBoard(boardPos, boardSize);

  for (const auto &piece : d->pieces) {
    auto pos = boardPos + static_cast<Vec2f>(piece.pos) * (width / 4.f);
    drawPiece(pos, piece.value, width / 4, width / 4);
  }
}

// TODO: sort according to movement direction first
// TODO: move until dead end
bool Game2048App::moveAll(const ci::Vec2i &offset) {
  bool moved = false;
  for (auto &iter = begin(d->pieces); iter != end(d->pieces); ++iter) {
    auto& p = *iter;
    auto pos = p.pos + offset;
    if (pos.x < 0 || pos.x > 4 || pos.y < 0 || pos.y > 4)
      continue;
    if (!isOccupied(pos)) {
      p.pos = pos;
      moved = true;
      continue;
    }
    auto& comb = at(pos);
    if (comb.value == p.value) {
      comb.value *= 2;
      iter = d->pieces.erase(iter);
      moved = true;
    }
  }
  return moved; 
}

void Game2048App::spawn() {
  if (d->pieces.size() >= 16)
    return;

  int value = 2;
  if (Random::next(1, 10) <= 3) {
    value = 4;
  }
  auto freeSpaces = getFreeSpaces();
  int ifs = Random::next(freeSpaces.size() - 1);
  auto pos = freeSpaces[ifs];
  d->pieces.push_back({ value, pos });
}

vector<Vec2i> Game2048App::getFreeSpaces() const {
  vector<Vec2i> freeSpaces;
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      if (isOccupied({ r, c }))
        continue;
      freeSpaces.push_back({ r, c });
    }
  }
  return freeSpaces;
}

Piece &Game2048App::at(const ci::Vec2i &pos) {
  return *find_if(begin(d->pieces), end(d->pieces),
                  [&](const Piece &p) { return p.pos == pos; });
}

bool Game2048App::isOccupied(const Vec2i &pos) const {
  return find_if(begin(d->pieces), end(d->pieces), [&](const Piece &p) {
           return p.pos == pos;
         }) != end(d->pieces);
}

void Game2048App::drawPiece(const ci::Vec2f &pos, int value, float width,
                            float height) const {
  Rectf pieceRect = { pos, pos + Vec2f{ width, height } };
  gl::color(Color::hex(0xDDDDDD00));
  gl::drawSolidRoundedRect(pieceRect, 5.f);
  TextBox tbox = TextBox()
                     .alignment(TextBox::CENTER)
                     .size(Vec2f(width, height))
                     .font(d->font)
                     .text(to_string(value));
  tbox.setColor(Color(1.0f, 0.65f, 0.35f));
  tbox.setBackgroundColor(ColorA(1, 1, 1, 0));
  gl::enableAlphaBlending();
  gl::draw(gl::Texture(tbox.render()), pieceRect);
  gl::disableAlphaBlending();
}

void Game2048App::drawBoard(const ci::Vec2f &pos, const ci::Vec2f &size) const {
  gl::color(Color::white());
  gl::drawSolidRoundedRect({ pos, pos + size }, 5.f);
}

CINDER_APP_BASIC(Game2048App, RendererGl);
