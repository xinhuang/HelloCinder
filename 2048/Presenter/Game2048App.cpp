#include "Game2048App.h"

#include "../Model/Piece.h"

#include "Config.h"
#include "Environment.h"
#include "Cell.h"

#include "../Util/Random.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

struct Game2048App::Data {
  vector<unique_ptr<Cell> > cells;
  Font font;
};

Game2048App::Game2048App() : d(make_unique<Data>()) {
  for (int c = 0; c < Config::SIZE; ++c)
    for (int r = 0; r < Config::SIZE; ++r)
      d->cells.push_back(make_unique<Cell>(Vec2i{ r, c }));
}

Game2048App::~Game2048App() {}

void Game2048App::setup() {
  setFrameRate(30);
  float width =
      static_cast<float>(min(getWindowHeight(), getWindowWidth())) - 14;
  Environment::instance().setCellSize(
      { width / Config::SIZE, width / Config::SIZE });
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
  float height = width;
  Vec2f boardPos{(getWindowWidth() - width) / 2.f,
                 (getWindowHeight() - width) / 2.f, };

  Vec2f boardSize{ width, width, };
  drawBoard(boardPos, boardSize);

  for (const auto &cell : d->cells) {
    auto pos =
        boardPos + static_cast<Vec2f>(cell->pos()) * (width / Config::SIZE);
    Rectf rect = { pos,
                   pos + Vec2f{ width / Config::SIZE, height / Config::SIZE } };
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::setViewport(getWindowBounds());
    gl::setMatricesWindow(getWindowSize());
    cell->draw(rect);
    gl::disableAlphaBlending();
  }
}

void Game2048App::resize() {
  float width =
      static_cast<float>(min(getWindowHeight(), getWindowWidth())) - 14;
  Environment::instance().setCellSize(
      { width / Config::SIZE, width / Config::SIZE });
}

bool Game2048App::moveAll(const ci::Vec2i &dir) {
  bool moved = false;
  auto xs = buildTraversals(Config::SIZE, dir.x);
  auto ys = buildTraversals(Config::SIZE, dir.y);

  for (auto x : xs) {
    for (auto y : ys) {
      auto &p = at({ x, y })->piece();
      if (!p)
        continue;
      moved = moveToFurthest({ x, y }, dir) || moved;
    }
  }

  for (auto &c : d->cells) {
    auto &p = c->piece();
    if (!p)
      continue;
    if (p->merged) {
      p->value *= 2;
      p->merged = nullptr;
    }
  }

  return moved;
}

bool Game2048App::moveToFurthest(Vec2i src, const Vec2i &dir) {
  auto within_boundary = [](const Vec2i &pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.x < Config::SIZE &&
           pos.y < Config::SIZE;
  };
  auto this_value = at(src)->value();
  auto dst = src + dir;
  for (; within_boundary(dst); dst += dir) {
    auto& cell = at(dst);
    if (!cell->piece())
      continue;
    if (cell->value() != this_value || cell->piece()->merged)
      break;
  }
  dst -= dir;
  if (src != dst) {
    if (!at(dst)->piece())
      at(src)->moveTo(*at(dst));
    else
      at(src)->mergeTo(*at(dst));
    return true;
  }
  return false;
}

void Game2048App::clear(const ci::Vec2i &pos) {
  auto &p = at(pos);
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
  at(pos)->place(make_unique<Piece>(value, pos));
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

unique_ptr<Cell> &Game2048App::at(const ci::Vec2i &pos) {
  assert(pos.x < Config::SIZE && pos.y < Config::SIZE && pos.x >= 0 &&
         pos.y >= 0);
  return d->cells[pos.y * Config::SIZE + pos.x];
}

const unique_ptr<Cell> &Game2048App::at(const ci::Vec2i &pos) const {
  assert(pos.x < Config::SIZE && pos.y < Config::SIZE && pos.x >= 0 &&
         pos.y >= 0);
  return d->cells[pos.y * Config::SIZE + pos.x];
}

bool Game2048App::isOccupied(const Vec2i &pos) const {
  assert(pos.x < Config::SIZE && pos.y < Config::SIZE && pos.x >= 0 &&
         pos.y >= 0);
  return d->cells[pos.y * Config::SIZE + pos.x]->piece() != nullptr;
}

void Game2048App::drawBoard(const ci::Vec2f &pos, const ci::Vec2f &size) const {
  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(getWindowSize());
  gl::color(Color::white());
  gl::drawSolidRoundedRect({ pos, pos + size }, 5.f);
}

CINDER_APP_BASIC(Game2048App, RendererGl);
