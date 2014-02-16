#include "stdafx.h"
#include "Universe.h"

#include <limits>
#include <future>

using namespace std;

bool Universe::isSilent() const {
  for (const auto &c : cells_) {
    if (!c.isDead())
      return false;
  }
  return true;
}

void Universe::add(const Cell &cell) {
  auto r = cells_.insert(cell);
  if (!r.second) {
    cells_.erase(r.first);
    cells_.insert(cell);
  }
}

void Universe::addn(const Cell& cell) {
  add(cell);
  if (cell.isDead())
    return;
  for (const auto& n : neighbors(cell.pos())) {
    if (!alive(n))
      add({ n, CellState::DEAD });
  }
}

void Universe::createIfNonexists(const Point &p) {
  cells_.insert({ p });
}

void Universe::nextGeneration(Universe &u) const {
  u.clear();
  for (const auto &cell : cells_) {
    const auto &pos = cell.pos();
    auto nextCell = rule_.nextGeneration(
        cell, (*this)[{ pos.x - 1, pos.y - 1 }], (*this)[{ pos.x - 1, pos.y }],
        (*this)[{ pos.x - 1, pos.y + 1 }], (*this)[{ pos.x, pos.y - 1 }],
        (*this)[{ pos.x, pos.y + 1 }], (*this)[{ pos.x + 1, pos.y - 1 }],
        (*this)[{ pos.x + 1, pos.y }], (*this)[{ pos.x + 1, pos.y + 1 }]);
    if (!nextCell.isDead()) {
      u.add(nextCell);
    }
  }

  vector<Cell> toAdd;
  for (const auto cell : u.cells_) {
    const auto& pos = cell.pos();
    if (!u.alive({ pos.x - 1, pos.y - 1 }))
      toAdd.emplace_back(pos.x - 1, pos.y - 1);
    if (!u.alive({ pos.x, pos.y - 1 }))
      toAdd.emplace_back(pos.x, pos.y - 1);
    if (!u.alive({ pos.x + 1, pos.y - 1 }))
      toAdd.emplace_back(pos.x + 1, pos.y - 1);

    if (!u.alive({ pos.x - 1, pos.y }))
      toAdd.emplace_back(pos.x - 1, pos.y);
    if (!u.alive({ pos.x + 1, pos.y }))
      toAdd.emplace_back(pos.x + 1, pos.y);

    if (!u.alive({ pos.x - 1, pos.y + 1 }))
      toAdd.emplace_back(pos.x - 1, pos.y + 1);
    if (!u.alive({ pos.x, pos.y + 1 }))
      toAdd.emplace_back(pos.x, pos.y + 1);
    if (!u.alive({ pos.x + 1, pos.y + 1 }))
      toAdd.emplace_back(pos.x + 1, pos.y + 1);
  }

  for (const auto& c : toAdd)
    u.add(c);
}

bool Universe::tryAdd(int x, int y) {
  Cell c{ { x, y }, CellState::DEAD };
  if (contains(c))
    return false;
  cells_.insert(c);
  return true;
}

vector<Point> Universe::neighbors(const Point &pos) const {
  return { { pos.x - 1, pos.y - 1 },
           { pos.x - 1, pos.y },
           { pos.x - 1, pos.y + 1 },
           { pos.x, pos.y - 1 },
           { pos.x, pos.y + 1 },
           { pos.x + 1, pos.y - 1 },
           { pos.x + 1, pos.y },
           { pos.x + 1, pos.y + 1 }, };
}

vector<Cell> Universe::neighborCells(const Cell &cell) const {
  const auto &pos = cell.pos();
  return {
    (*this)[{ pos.x - 1, pos.y - 1 }], (*this)[{ pos.x - 1, pos.y }],
    (*this)[{ pos.x - 1, pos.y + 1 }], (*this)[{ pos.x, pos.y - 1 }],
    (*this)[{ pos.x, pos.y + 1 }],     (*this)[{ pos.x + 1, pos.y - 1 }],
    (*this)[{ pos.x + 1, pos.y }],     (*this)[{ pos.x + 1, pos.y + 1 }],
  };
}

const Cell &Universe::operator[](const Point &pos) const {
  Cell default { pos, CellState::DEAD };
  auto iter = cells_.find(default);
  if (iter == cells_.end())
    return Cell::DEAD_CELL;
  return *iter;
}

Universe Universe::bigBang(int width, int height, double rate, function<int(int)> rand) {
  Universe u;
  for (int i = 0; i < width * height * rate; ++i) {
    int x = rand(width);
    int y = rand(height);
    u.addn({ x, y }, CellState::ALIVE);
  }
  return u;
}

Universe Universe::glider() {
  Universe u;
  u.addn({ 0, 1 }, CellState::ALIVE);
  u.addn({ 1, 2 }, CellState::ALIVE);
  u.addn({ 2, 0 }, CellState::ALIVE);
  u.addn({ 2, 1 }, CellState::ALIVE);
  u.addn({ 2, 2 }, CellState::ALIVE);
  return u;
}

ostream& operator<<(ostream& os, const Universe& u) {
  int minx = numeric_limits<int>::max();
  int miny = numeric_limits<int>::max();
  int maxx = numeric_limits<int>::min();
  int maxy = numeric_limits<int>::min();
  for (auto& iter = u.begin(); iter != u.end(); ++iter) {
    if (iter->isDead())
      continue;
    if (iter->pos().x < minx)
      minx = iter->pos().x;
    if (iter->pos().y < miny)
      miny = iter->pos().y;
    if (iter->pos().x > maxx)
      maxx = iter->pos().x;
    if (iter->pos().y > maxy)
      maxy = iter->pos().y;
  }
  if (minx > maxx || miny > maxy)
    return os;
  for (int y = miny; y <= maxy; ++y) {
    for (int x = minx; x <= maxx; ++x) {
      if (u.alive({ x, y }))
        os << "*";
      else
        os << ".";
    }
    os << endl;
  }
  return os;
}

istream& operator>>(istream& is, Universe& u) {
  u.clear();
  string s;
  int y = 0;
  while (getline(is, s)) {
    int x = 0;
    for (const auto& c : s) {
      u.add({ { x++, y }, CellState::ALIVE });
    }
    ++y;
  }
  return is;
}

bool operator==(const Universe& lhs, const Universe& rhs) {
  for (auto& iter = lhs.begin(); iter != lhs.end(); ++iter) {
    if (!rhs.contains(*iter))
      return false;
  }
  return true;
}
