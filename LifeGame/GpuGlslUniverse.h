#pragma once
#include "Universe.h"

#include <cinder/gl/GlslProg.h>
#include <cinder/gl/fbo.h>
#include <cinder/Channel.h>

#include <string>

class GpuGlslUniverse : public IUniverse {
  int ifbo_ = 0;
  mutable ci::gl::Fbo fbos_[2];
  ci::gl::GlslProgRef mShader_;
  std::unique_ptr<ci::Channel> channel_;

public:
  GpuGlslUniverse(int width, int height);

  void next() final;
  ci::gl::Texture render() const final;

  std::string name() const final;
  void add(const ci::Vec2i &p) final;
  int width() const final;
  int height() const final;
  int size() const final;
};
