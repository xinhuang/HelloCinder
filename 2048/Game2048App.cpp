#include "Game2048App.h"

#include "Environment.h"
#include "Random.h"
#include "Piece.h"
#include "Config.h"
#include "PieceAnimation.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

class Cell {
  const ci::Vec2i pos_;
  std::unique_ptr<Piece> piece_;
  std::unique_ptr<IRenderable> animation_;

public:
  Cell(const ci::Vec2i &position) : pos_(position) {
    animation_ = std::make_unique<EmptyCellAnimation>();
  }

  const std::unique_ptr<Piece> &piece() const { return piece_; }
  const ci::Vec2i &pos() const { return pos_; }

  void draw(const ci::Rectf &rect) { animation_->draw(rect); }
  void place(std::unique_ptr<Piece> &&p) {
    piece_ = std::move(p);
    animation_ = std::make_unique<PlacePieceAnimation>(*piece_);
  }
  void moveTo(Cell &cell) {
    cell.piece_ = std::move(piece_);
    cell.animation_ = std::make_unique<MovePieceAnimation>(*cell.piece_);
    animation_ = std::make_unique<EmptyCellAnimation>();
  }
  void mergeTo(Cell &cell) {
    cell.piece_->merged = std::move(piece_);
    cell.animation_ = std::make_unique<MergeAnimation>(*cell.piece_);
    animation_ = std::make_unique<EmptyCellAnimation>();
  }
};

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

bool Game2048App::moveToFurthest(Vec2i srcpos, const Vec2i &dir) {
  bool moved = false;
  do {
    auto dstpos = srcpos + dir;
    if (dstpos.x < 0 || dstpos.y < 0 || dstpos.x >= 4 || dstpos.y >= 4)
      return moved;
    auto &src = at(srcpos)->piece();
    assert(src);
    auto &dst = at(dstpos)->piece();
    if (dst && (dst->value != src->value || dst->merged))
      return moved;

    src->pos = dstpos;
    if (!dst) {
      at(srcpos)->moveTo(*at(dstpos));
    } else {
      at(srcpos)->mergeTo(*at(dstpos));
      return true;
    }
    moved = true;
    srcpos = dstpos;
  } while (true);
  return moved;
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
