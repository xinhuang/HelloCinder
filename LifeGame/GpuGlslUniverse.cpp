#include "stdafx.h"
#include "GpuGlslUniverse.h"

#include <cinder/app/App.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>

using namespace std;

#include "Resources.h"

GpuGlslUniverse::GpuGlslUniverse(int width, int height) {
  try {
    mShader_ = gl::GlslProg::create(loadResource(RES_PASSTHRU_VERT),
      loadResource(RES_LIFEGAME_FRAG));
  } catch (gl::GlslProgCompileExc &exc) {
    std::cout << "Shader compile error: " << std::endl;
    std::cout << exc.what();
  } catch (...) {
    std::cout << "Unable to load shader" << std::endl;
  }

  fbos_[0] = gl::Fbo(width, height, false);
  fbos_[1] = gl::Fbo(width, height, false);
}

void GpuGlslUniverse::next() {
  gl::Fbo& src = fbos_[ifbo_];
  gl::Fbo& dst = fbos_[1 - ifbo_];

  if (channel_) {
    dst.bindFramebuffer();
    gl::setViewport(dst.getBounds());
    gl::setMatricesWindow(dst.getSize(), false);
    gl::clear();
    gl::color(Color::white());
    gl::draw(gl::Texture(*channel_));
    dst.unbindFramebuffer();
    channel_.release();
    ifbo_ = (ifbo_ + 1) % 2;
    return;
  }

  dst.bindFramebuffer();
  gl::setViewport(dst.getBounds());
  gl::setMatricesWindow(dst.getSize(), false);
  gl::clear();
  gl::color(Color::white());

  src.bindTexture();
  mShader_->bind();
  mShader_->uniform("pixel",
    Vec2f::one() / static_cast<Vec2f>(src.getSize()));
  mShader_->uniform("src", 0);
  gl::drawSolidRect(dst.getBounds());
  mShader_->unbind();
  src.unbindTexture();
  dst.unbindFramebuffer();

  ifbo_ = (ifbo_ + 1) % 2;
}

ci::gl::Texture GpuGlslUniverse::render() const {
  auto& dst = fbos_[1 - ifbo_];
  auto tex = dst.getTexture();
  tex.setFlipped();
  return tex;
}

std::string GpuGlslUniverse::name() const {
  return "GPU GLSL";
}

void GpuGlslUniverse::add(const ci::Vec2i &p) {
  if (!channel_) {
    channel_ = make_unique<Channel>(fbos_[ifbo_].getWidth(), fbos_[ifbo_].getHeight());
    auto p = channel_->getData();
    fill(p, p + channel_->getRowBytes() * channel_->getHeight(), 0x00);
  }
  channel_->setValue(p, 0xFF);
}

int GpuGlslUniverse::width() const {
  return fbos_[ifbo_].getWidth();
}

int GpuGlslUniverse::height() const {
  return fbos_[ifbo_].getHeight();
}
int GpuGlslUniverse::size() const {
  return width() * height();
}
