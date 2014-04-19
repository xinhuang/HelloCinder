#pragma once

#include <memory>

#include <cinder/Cinder.h>
#include <cinder/gl/Texture.h>
#include <cinder/gl/Fbo.h>

// TODO: should be cell renderer
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

  void draw(const Piece& p, ci::Rectf rect);
  void draw(const int value, ci::Rectf rect);
  ci::gl::TextureRef render(const int value, const ci::Vec2f& size);
};