#pragma once

#include "../View/animation/Animation.h"

#include <cinder/Rect.h>

#include <memory>

class Piece {
public:
  std::unique_ptr<Piece> merged;
  int value;

  Piece(int v) : value(v) {}
  Piece(Piece &&p) : value(p.value), merged(std::move(p.merged)) {}
};
