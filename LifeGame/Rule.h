#pragma once

#include <vector>

#include "Cell.h"

class Rule {
public:
  Cell nextGeneration(Cell cell, std::vector<Cell> neighbors) const;
};


