#include "stdafx.h"
#include "Rule.h"

#include <algorithm>
using namespace std;

Cell Rule::nextGeneration(Cell cell, vector<Cell> neighbors) const {
  auto p = partition(begin(neighbors), end(neighbors),
                     [](const Cell &c) { return !c.isDead(); });
  int count = p - begin(neighbors);
  if (count == 2)
    return cell;
  if (count == 3) {
    cell.setState(CellState::ALIVE);
    return cell;
  }
  cell.setState(CellState::DEAD);
  return cell;
}

Cell Rule::nextGeneration(Cell cell, const Cell &n0, const Cell &n1,
                          const Cell &n2, const Cell &n3, const Cell &n4,
                          const Cell &n5, const Cell &n6,
                          const Cell &n7) const {
  int count = 0;
  if (!n0.isDead())
    ++count;
  if (!n1.isDead())
    ++count;
  if (!n2.isDead())
    ++count;
  if (!n3.isDead())
    ++count;
  if (!n4.isDead())
    ++count;
  if (!n5.isDead())
    ++count;
  if (!n6.isDead())
    ++count;
  if (!n7.isDead())
    ++count;
  if (count == 2)
    return cell;
  if (count == 3) {
    cell.setState(CellState::ALIVE);
    return cell;
  }
  cell.setState(CellState::DEAD);
  return cell;
}

bool Rule::nextGeneration(bool cur, bool n0, bool n1, bool n2, bool n3, bool n4,
                          bool n5, bool n6, bool n7) const {
  int count = 0;
  if (n0)
    ++count;
  if (n1)
    ++count;
  if (n2)
    ++count;
  if (n3)
    ++count;
  if (n4)
    ++count;
  if (n5)
    ++count;
  if (n6)
    ++count;
  if (n7)
    ++count;
  if (count == 2)
    return cur;
  else if (count == 3)
    cur = true;
  else
    cur = false;
  return cur;
}
