#pragma once

#include <memory>

#include <cinder/Cinder.h>
#include <cinder/gl/Texture.h>

class Piece {
public:
  std::unique_ptr<Piece> merged;
  int value;
  ci::Vec2i pos;

  Piece(int v, const ci::Vec2i &p) : value(v), pos(p) {}
  Piece(Piece &&p) : value(p.value), pos(p.pos), merged(std::move(p.merged)) {}
};
