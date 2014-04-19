#pragma once

#include "IRenderable.h"
#include "PieceRenderer.h"

#include <memory>

#include <cinder/gl/Texture.h>
#include <cinder/Rect.h>

class Animation : public IRenderable {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Animation();
  ~Animation() override;

  Animation(Animation &&other);
  Animation &operator=(Animation &&other);

  Animation &&zoom(ci::gl::TextureRef& tex, float beginScale, float endScale, int nframes);

  void draw(const ci::Rectf &rect) override;
};
