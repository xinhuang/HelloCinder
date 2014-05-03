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
  ci::gl::TextureRef render(const int value, const ci::Vec2f &size);

public:
  static CellRenderer &instance() {
    static CellRenderer pr;
    return pr;
  }

  ci::gl::TextureRef wrender(int value, const ci::Vec2f &size) {
    return render(value, size);
  }
};