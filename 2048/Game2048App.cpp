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
  unique_ptr<Piece> merged;
  int value;
  Vec2i pos;

  Piece(int v, const Vec2i& p) : value(v), pos(p) {}
  Piece(Piece&& p) : value(p.value), pos(p.pos), merged(move(p.merged)) {}
};

namespace Config {
  const int SIZE = 4;
}

struct Game2048App::Data {
  vector<unique_ptr<Piece>> pieces = vector<unique_ptr<Piece>>(Config::SIZE * Config::SIZE);
  Font font;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  d->font = Font("Arial", 60);
  spawn();
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
    if (!piece) 
      continue;
    auto pos = boardPos + static_cast<Vec2f>(piece->pos) * (width / 4.f);
    drawPiece(pos, piece->value, width / 4, width / 4);
  }
}

bool Game2048App::moveAll(const ci::Vec2i &dir) {
  bool moved = false;
  auto xs = buildTraversals(4, dir.x);
  auto ys = buildTraversals(4, dir.y);

  for (auto x : xs) {
    for (auto y : ys) {
      auto& piece = at({ x, y });
      if (!piece) 
        continue;
      moved = moveToFurthest({ x, y }, dir) || moved;
    }
  }

  for (auto& p : d->pieces) {
    if (!p)
      continue;
    if (p->merged) {
      p->value *= 2;
      p->merged = nullptr;
    }
  }

  return moved;
}

bool Game2048App::moveToFurthest(Vec2i srcpos, const Vec2i& dir) {
  bool moved = false;
  do {
    auto dstpos = srcpos + dir;
    if (dstpos.x < 0 || dstpos.y < 0 || dstpos.x >= 4 || dstpos.y >= 4)
      return moved;
    auto& src = at(srcpos);
    assert(src);
    auto& dst = at(dstpos);
    if (dst && (dst->value != src->value || dst->merged))
      return moved;

    src->pos = dstpos;
    if (!dst) {
      dst = move(src);
    } else {
      dst->merged = move(src);
      return true;
    }
    moved = true;
    srcpos = dstpos;
  } while (true);
  return moved;
}

void Game2048App::clear(const ci::Vec2i& pos) {
  auto& p = at(pos);
  assert(p);
  p.reset();
}

vector<int> Game2048App::buildTraversals(int max, int dir) const {
  vector<int> r;
  for (int i = 0; i < max; ++i) {
    r.push_back(i);
  }
  if (dir == 1)
    r = { rbegin(r), rend(r) };
  return r;
}

void Game2048App::spawn() {
  int value = 2;
  if (Random::next(1, 10) <= 3) {
    value = 4;
  }
  auto freeSpaces = getFreeSpaces();
  assert(freeSpaces.size() > 0);
  int ifs = Random::next(freeSpaces.size() - 1);
  auto pos = freeSpaces[ifs];
  at(pos) = make_unique<Piece>(value, pos);
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

unique_ptr<Piece>& Game2048App::at(const ci::Vec2i &pos) {
  assert(pos.x < Config::SIZE && pos.y < Config::SIZE && pos.x >= 0 && pos.y >= 0);
  return d->pieces[pos.y * Config::SIZE + pos.x];
}

const unique_ptr<Piece>& Game2048App::at(const ci::Vec2i &pos) const {
  assert(pos.x < Config::SIZE && pos.y < Config::SIZE && pos.x >= 0 && pos.y >= 0);
  return d->pieces[pos.y * Config::SIZE + pos.x];
}

bool Game2048App::isOccupied(const Vec2i &pos) const {
  assert(pos.x < Config::SIZE && pos.y < Config::SIZE && pos.x >= 0 && pos.y >= 0);
  return d->pieces[pos.y * Config::SIZE + pos.x] != nullptr;
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
