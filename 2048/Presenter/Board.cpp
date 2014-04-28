#include "Board.h"

#include "Cell.h"
#include "Scene.h"
#include "Config.h"

#include "..\Util\Random.h"

#include <vector>
#include <cassert>

using namespace std;

using namespace ci;

struct Board::Data {
  int width;
  int height;
  vector<Cell> cells;
  vector<IBoardEventListener*> listeners;
};

Board::Board() : d(make_unique<Data>()) {}

Board::Board(int width, int height) : Board() {
  d->width = width;
  d->height = height;
  for (int c = 0; c < width; ++c)
    for (int r = 0; r < height; ++r)
      d->cells.push_back({ Vec2i{ r, c } });
}

Board &Board::operator=(Board &&board) {
  d = std::move(board.d);
  return *this;
}

Board::~Board() {}

int Board::width() const { return d->width; }
int Board::height() const { return d->height; }

bool Board::moves_available() const {
  if (freespaces().size() > 0)
    return true;

  const int dx[] = { 0, 1 };
  const int dy[] = { 1, 0 };
  for (int x = 0; x < width(); ++x) {
    for (int y = 0; y < height(); ++y) {
      assert(at({ x, y }).piece());
      assert(at({ x, y }).piece()->merged == nullptr);

      for (int i = 0; i < 2; ++i) {
        if (x + dx[i] >= width() || y + dy[i] >= height())
          continue;
        assert(at({ x + dx[i], y + dy[i] }).piece());
        assert(at({ x + dx[i], y + dy[i] }).piece()->merged == nullptr);
        if (at({ x, y }).piece()->value ==
            at({ x + dx[i], y + dy[i] }).piece()->value)
          return true;
      }
    }
  }

  return false;
}

void Board::draw(const Rectf &rect) {
  gl::color(Color::hex(Config::BOARD_COLOR));
  gl::drawSolidRoundedRect(rect, 5.f);

  for (auto &cell : d->cells) {
    cell.drawBackground(Scene::cellRect(cell.coord()));
  }
  for (auto &cell : d->cells) {
    if (cell.piece())
      cell.draw(Scene::cellRect(cell.coord()));
  }
}

bool Board::slide(const ci::Vec2i &dir) {
  bool moved = false;
  auto xs = traversal(width(), dir.x);
  auto ys = traversal(height(), dir.y);

  for (auto x : xs) {
    for (auto y : ys) {
      auto &p = at({ x, y }).piece();
      if (!p)
        continue;
      moved = slide({ x, y }, dir) || moved;
    }
  }

  for (auto &c : d->cells) {
    auto &p = c.piece();
    if (!p)
      continue;
    if (p->merged) {
      onPieceMerged(*(p->merged), *p);
      p->value++;
      p->merged = nullptr;
    }
  }

  return moved;
}

bool Board::slide(Vec2i src, const Vec2i &dir) {
  auto within_boundary = [this](const Vec2i &pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.x < width() && pos.y < height();
  };
  auto this_value = at(src).value();
  auto dst = src + dir;
  for (; within_boundary(dst); dst += dir) {
    auto &cell = at(dst);
    if (!cell.piece())
      continue;
    if (cell.value() != this_value || cell.piece()->merged)
      break;
  }
  dst -= dir;
  if (src != dst) {
    if (!at(dst).piece())
      at(src).moveTo(at(dst));
    else
      at(src).mergeTo(at(dst));
    return true;
  }
  return false;
}

vector<int> Board::traversal(int max, int dir) const {
  vector<int> r;
  for (int i = 0; i < max; ++i) {
    r.push_back(i);
  }
  if (dir == 1)
    r = { rbegin(r), rend(r) };
  return r;
}

void Board::spawn() {
  int value = 1;
  if (Random::next(1, 10) <= 3) {
    value = 2;
  }
  auto freeSpaces = freespaces();
  assert(freeSpaces.size() > 0);
  int ifs = Random::next(freeSpaces.size() - 1);
  auto pos = freeSpaces[ifs];
  at(pos).place(make_unique<Piece>(value));
}

vector<Vec2i> Board::freespaces() const {
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

bool Board::isOccupied(const ci::Vec2i &coord) const {
  return at(coord).piece() != nullptr;
}

Cell &Board::at(const ci::Vec2i &coord) {
  assert(coord.x < width() && coord.y < height() && coord.x >= 0 &&
         coord.y >= 0);
  return d->cells[coord.y * width() + coord.x];
}

const Cell &Board::at(const ci::Vec2i &coord) const {
  assert(coord.x < width() && coord.y < height() && coord.x >= 0 &&
         coord.y >= 0);
  return d->cells[coord.y * width() + coord.x];
}

void Board::place(const ci::Vec2i &coord, std::unique_ptr<Piece> &&p) {
  at(coord).place(move(p));
}

void Board::addListener(IBoardEventListener &listener) {
  d->listeners.push_back(&listener);
}

void Board::onPieceMerged(const Piece &from, const Piece &to) {
  for (auto& listener : d->listeners) {
    listener->onPieceMerged(from, to);
  }
}
