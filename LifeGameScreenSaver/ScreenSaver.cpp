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

ScreenSaver::ScreenSaver() {}

void ScreenSaver::setup() {
  font_ = TextureFont::create(Font("Consolas", 20));
  u_ = bigBang<universe_t>(getWindowWidth(), getWindowHeight());
  sysinfo_.init(*u_);
  ippInit();
}

void ScreenSaver::draw() {
  gl::draw(u_->render());
  font_->drawString(sysinfo_.msg(*this), getWindowBounds());
}

void ScreenSaver::update() {
  if (sysinfo_.elapsed() >= REFRESH_TIME) {
    u_ = bigBang<universe_t>(getWindowWidth(), getWindowHeight());
	sysinfo_.init(*u_);
  } else {
    sysinfo_.onPreGen(*u_);

    u_->next();

    sysinfo_.onPostGen(*u_);
  }
}

CINDER_APP_SCREENSAVER(ScreenSaver, RendererGl)
