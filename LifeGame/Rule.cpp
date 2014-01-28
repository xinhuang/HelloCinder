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
