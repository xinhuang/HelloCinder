#pragma once

#include "IRenderable.h"

class NullRenderer : public IRenderable {
public:
  void draw(const ci::Rectf &rect, float alpha) final {}
};
