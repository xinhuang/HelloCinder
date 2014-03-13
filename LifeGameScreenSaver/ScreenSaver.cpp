#include "ScreenSaver.h"

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <map>
#include <random>

using namespace std;

#include "../LifeGame/vax.h"

#include <cinder/cinder.h>

#pragma comment(lib, "Scrnsavw.lib")
#pragma comment(lib, "comctl32.lib")

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

ScreenSaver::ScreenSaver() {
}

void ScreenSaver::setup() {
  font_ = TextureFont::create(Font("Consolas", 20));
  now_ = Universe::bigBang(getWindowWidth(), getWindowHeight());
  next_ = Universe::bigBang(getWindowWidth(), getWindowHeight());
  sysinfo_.init(now_);
  ippInit();
}

void ScreenSaver::draw() {
  gl::draw(now_.texture());
  font_->drawString(sysinfo_.msg(), getWindowBounds());
}

void ScreenSaver::update() {
  if (sysinfo_.elapsed() >= REFRESH_TIME) {
    now_ = Universe::bigBang(getWindowWidth(), getWindowHeight());
    next_ = Universe::bigBang(getWindowWidth(), getWindowHeight());
    sysinfo_.init(now_);
  } else {
    sysinfo_.onPreGen(now_);

    now_.next(next_);
    swap(now_, next_);

    sysinfo_.onPostGen(now_);
  }
}

CINDER_APP_SCREENSAVER(ScreenSaver, RendererGl)
