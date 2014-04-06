#pragma once

#include <memory>

#include <cinder/Cinder.h>
#include <cinder/gl/Texture.h>
#include <cinder/gl/Fbo.h>

class Piece;

class PieceRenderer {
  struct Data;
  std::unique_ptr<Data> d;

  PieceRenderer();
  ~PieceRenderer();

public:
  static PieceRenderer &instance() {
    static PieceRenderer pr;
    return pr;
  }

  void PieceRenderer::draw(const Piece& p, ci::Rectf rect);
};