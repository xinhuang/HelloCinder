#pragma once

#include <memory>

#include <cinder/Rect.h>
#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>

class CellRenderer {
  struct Data;
  std::unique_ptr<Data> d;

  CellRenderer();
  ~CellRenderer();

public:
  static CellRenderer &instance() {
    static CellRenderer pr;
    return pr;
  }

  void draw(const int value, ci::Rectf rect);
  ci::gl::TextureRef render(const int value, const ci::Vec2f& size);
};