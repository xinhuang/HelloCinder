#include "stdafx.h"
#include "Universe.h"

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

  for (const auto &n : neighbors(cell.pos())) {
    createIfNonexists(n);
  }
}

void Universe::createIfNonexists(const Point &p) {
  cells_.insert({ p });
}

Universe Universe::nextGeneration() const {
  Universe next(cells_.size());
  for (const auto &cell : cells_) {
    auto ns = neighborCells(cell);
    auto nextCell = rule_.nextGeneration(cell, ns);
    if (!nextCell.isDead()) {
      next.add(nextCell);
    }
  }
  return next;
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
