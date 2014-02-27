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

void Universe::add(const Point &p, CellState s) {
  if (s == CellState::DEAD) {
    cells_.insert({ p, s });
    return;
  }

  auto r = cells_.insert({ p, s });
  if (!r.second)
    r.first->setState(s);
}

void Universe::addn(const Point &pos, CellState state) {
  add(pos, state);
  if (state == CellState::DEAD)
    return;
  add({ pos.x - 1, pos.y - 1 }, CellState::DEAD);
  add({ pos.x, pos.y - 1 }, CellState::DEAD);
  add({ pos.x + 1, pos.y - 1 }, CellState::DEAD);

  add({ pos.x - 1, pos.y }, CellState::DEAD);
  add({ pos.x + 1, pos.y }, CellState::DEAD);

  add({ pos.x - 1, pos.y + 1 }, CellState::DEAD);
  add({ pos.x, pos.y + 1 }, CellState::DEAD);
  add({ pos.x + 1, pos.y + 1 }, CellState::DEAD);
}

auto Universe::erase(const Point& c) ->
#if defined CONCURRENT_GENERATION
decltype(container_t().unsafe_erase(c))
#else
decltype(container_t().erase(c))
#endif 
{
#if defined CONCURRENT_GENERATION
  return cells_.unsafe_erase(c);
#else
  return cells_.erase(c);
#endif
}

void Universe::nextGeneration(Universe &u) const {
#if defined CONCURRENT_GENERATION
  parallelNextGeneration(u);
#else
  sequentialNextGeneration(u);
#endif
}

void Universe::parallelNextGeneration(Universe &u) const {
  u.clear();

  const double MIN_CHUNK = 1000.0;
  const double trunkSize = max(cells_.size() / 4.0, MIN_CHUNK);
  const int nTrunk = static_cast<int>(ceil(cells_.size() / trunkSize));

  auto generate = [=, &u](int trunk) {
    auto iter = cells_.begin();
    advance(iter, trunk * trunkSize);
    for (int i = 0; i < trunkSize && iter != cells_.end(); ++i) {
      const auto &cell = *iter;
      const auto &pos = cell.pos();
      auto nextCell = rule_.nextGeneration(
          cell, (*this)[{ pos.x - 1, pos.y - 1 }],
          (*this)[{ pos.x - 1, pos.y }], (*this)[{ pos.x - 1, pos.y + 1 }],
          (*this)[{ pos.x, pos.y - 1 }], (*this)[{ pos.x, pos.y + 1 }],
          (*this)[{ pos.x + 1, pos.y - 1 }], (*this)[{ pos.x + 1, pos.y }],
          (*this)[{ pos.x + 1, pos.y + 1 }]);
      if (!nextCell.isDead()) {
        u.addn(pos, nextCell.state());
      }
      ++iter;
    }
  };

  vector<future<void> > fs;
  for (int trunk = 0; trunk < nTrunk - 1; ++trunk)
    fs.push_back(async(generate, trunk));
  generate(nTrunk - 1);

  for (auto &f : fs) {
    f.get();
  }
}

void Universe::sequentialNextGeneration(Universe &u) const {
  u.clear();
  for (const auto &cell : cells_) {
    const auto &pos = cell.pos();
    auto nextCell = rule_.nextGeneration(
        cell, (*this)[{ pos.x - 1, pos.y - 1 }], (*this)[{ pos.x - 1, pos.y }],
        (*this)[{ pos.x - 1, pos.y + 1 }], (*this)[{ pos.x, pos.y - 1 }],
        (*this)[{ pos.x, pos.y + 1 }], (*this)[{ pos.x + 1, pos.y - 1 }],
        (*this)[{ pos.x + 1, pos.y }], (*this)[{ pos.x + 1, pos.y + 1 }]);
    if (!nextCell.isDead()) {
      u.add(pos, nextCell.state());
    }
  }

  vector<Cell> toAdd;
  for (const auto cell : u.cells_) {
    const auto &pos = cell.pos();
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

  for (const auto &c : toAdd)
    u.add(c.pos(), c.state());
}

const Cell &Universe::operator[](const Point &pos) const {
  Cell default { pos, CellState::DEAD };
  auto iter = cells_.find(default);
  if (iter == cells_.end())
    return Cell::DEAD_CELL;
  return *iter;
}

Universe Universe::bigBang(int width, int height, double rate,
                           const function<int(int)> &rand) {
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

ostream &operator<<(ostream &os, const Universe &u) {
  int minx = numeric_limits<int>::max();
  int miny = numeric_limits<int>::max();
  int maxx = numeric_limits<int>::min();
  int maxy = numeric_limits<int>::min();
  for (auto &iter = u.begin(); iter != u.end(); ++iter) {
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

istream &operator>>(istream &is, Universe &u) {
  u.clear();
  string s;
  int y = 0;
  while (getline(is, s)) {
    int x = 0;
    for (const auto &c : s) {
      u.add({ x++, y }, CellState::ALIVE);
    }
    ++y;
  }
  return is;
}

bool operator==(const Universe &lhs, const Universe &rhs) {
  for (auto &iter = lhs.begin(); iter != lhs.end(); ++iter) {
    if (!rhs.contains(*iter))
      return false;
  }
  return true;
}
