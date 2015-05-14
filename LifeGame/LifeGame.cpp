#include "stdafx.h"
#include "LifeGame.h"

#include "Universe.h"
#include "GpuGlslUniverse.h"
#include "GameConfig.h"
#include "Random.h"

#include <cinder/cinder.h>
#include <cinder/gl/TextureFont.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

#include <vector>

using namespace std;

#include <omp.h>
#include "vax.h"

#if defined USE_TBB
#include <tbb/tbb.h>
using namespace tbb;
#endif

struct LifeGame::Data {
  bool suspend_ = false;
  bool dragging_ = false;
  bool displayInfo_ = true;
  float fixedFps_ = 0;

  ci::Vec2f offset_;
  ci::Vec2f mouseDownOffset_;
  ci::Area windowBounds_;
  ci::Vec2i windowSize_;
  ci::Font font_;

  int iuniverse_ = 0;
  vector<function<unique_ptr<IUniverse>(int, int)> > creators_;
  unique_ptr<IUniverse> u_;
  Sysinfo sysinfo_;

  const int FALLBACK = 0;
};

LifeGame::LifeGame() : d(make_unique<Data>()) {
  d->creators_.push_back(bigBang<CpuLoopUniverse>);
  d->creators_.push_back(bigBang<CpuLoopOmpUniverse>);
  d->creators_.push_back(bigBang<CpuAvxUniverse>);
  d->creators_.push_back(bigBang<CpuAvxOmpUniverse>);
#if !defined NO_IPP
  d->creators_.push_back(bigBang<CpuIppUniverse>);
  d->creators_.push_back(bigBang<CpuIppOmpUniverse>);
#endif // NO_IPP
  d->creators_.push_back(bigBang<GpuGlslUniverse>);
#if defined USE_TBB
  d->creators_.push_back(bigBang<CpuIppTbbUniverse>);
#endif // USE_TBB
}

void LifeGame::setup() {
  gl::disableVerticalSync();
  setFrameRate(99999.f);
  d->font_ = Font("Arial", 20);

  createUniverse(getWindowWidth(), getWindowHeight());

  ippInit();
} 

void LifeGame::createUniverse(int width, int height) {
  try {
    d->u_ = d->creators_[d->iuniverse_](width, height);
    d->sysinfo_.init(*(d->u_));
    d->offset_ = {};
  }
  catch (std::runtime_error &e) {
    d->u_ = d->creators_[d->FALLBACK](width, height);
    d->sysinfo_.init(*(d->u_), e.what());
    d->offset_ = {};
  }
}

void LifeGame::draw() {
  auto tex = d->u_->render();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gl::setViewport(d->windowBounds_);
  gl::setMatricesWindow(d->windowSize_);
  gl::draw(tex, d->windowBounds_);

  if (d->displayInfo_) {
    ci::TextLayout layout;
    gl::enableAlphaBlending();
	layout.setFont(d->font_);
	layout.setColor(ColorA::white());
	layout.addRightLine(d->sysinfo_.msg(*this));
	auto tex = layout.render(true);
	gl::color(ColorA(0.f, 0.f, 1.f, 0.5f));
	gl::drawSolidRect(tex.getBounds());
	gl::color(ColorA::white());
	gl::draw(tex);
	gl::disableAlphaBlending();
  }
}

void LifeGame::update() {
  if (!d->suspend_) {
    d->sysinfo_.onPreGen(*(d->u_));
    d->u_->next();
    d->sysinfo_.onPostGen(*(d->u_));
  }
}

void LifeGame::resize() {
  d->windowBounds_ = getWindowBounds();
  d->windowSize_ = getWindowSize();
}

void LifeGame::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_RETURN:
	createUniverse(getWindowWidth(), getWindowHeight());
    break;

  case KeyEvent::KEY_RIGHT:
    d->iuniverse_ = (d->iuniverse_ + 1) % d->creators_.size();
    createUniverse(getWindowWidth(), getWindowHeight());
    break;

  case KeyEvent::KEY_LEFT:
    d->iuniverse_ =
        (d->iuniverse_ + d->creators_.size() - 1) % d->creators_.size();
    createUniverse(getWindowWidth(), getWindowHeight());
    break;

  case KeyEvent::KEY_UP:
	  d->fixedFps_ += 1;
	  setFrameRate(d->fixedFps_);
	  d->sysinfo_.fixFps();
	  break;

  case KeyEvent::KEY_DOWN:
    d->fixedFps_ -= 1;
    if (d->fixedFps_ > 0) {
      setFrameRate(d->fixedFps_);
	  d->sysinfo_.fixFps();
    } else {
      d->fixedFps_ = 0;
      setFrameRate(99999.f);
	  d->sysinfo_.unfixFps();
    }
    break;

  case KeyEvent::KEY_ESCAPE:
    quit();
    break;

  case KeyEvent::KEY_SPACE:
    d->suspend_ = !d->suspend_;
    break;

  case KeyEvent::KEY_c:
    d->u_ = {};
    break;

  case KeyEvent::KEY_f:
    setFullScreen(!isFullScreen());
    break;

  case KeyEvent::KEY_HOME:
  case KeyEvent::KEY_r:
    d->offset_ = {};
    break;

  case KeyEvent::KEY_g:
    d->displayInfo_ = !d->displayInfo_;
    break;
  }
}

CINDER_APP_BASIC(LifeGame, RendererGl)
