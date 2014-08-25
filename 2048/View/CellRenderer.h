#pragma once

#include "animation/Slice.h"

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

  std::shared_ptr<animation::Slice> render(int value, const ci::Vec2f &size);
  void resize();
};