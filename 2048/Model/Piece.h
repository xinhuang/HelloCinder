#pragma once

#include <memory>

#include <cinder/Cinder.h>

class Piece {
public:
  std::unique_ptr<Piece> merged;
  int value;

  Piece(int v, const ci::Vec2i &p) : value(v) {}
  Piece(Piece &&p) : value(p.value), merged(std::move(p.merged)) {}
};
