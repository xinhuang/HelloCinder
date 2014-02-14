#pragma once

#include <vector>

#include "Cell.h"

class Rule {
public:
  Cell nextGeneration(Cell cell, std::vector<Cell> neighbors) const;
  Cell nextGeneration(Cell cell, const Cell &n0, const Cell &n1, const Cell &n2,
                      const Cell &n3, const Cell &n4, const Cell &n5,
                      const Cell &n6, const Cell &n7) const;
};


