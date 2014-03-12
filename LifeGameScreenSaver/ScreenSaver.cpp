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

#include "ScreenSaver.h"

#include <cinder/cinder.h>

#pragma comment(lib, "Scrnsavw.lib")
#pragma comment(lib, "comctl32.lib")

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

using namespace std;

ScreenSaver::ScreenSaver() {
}

void ScreenSaver::setup() {
  font_ = TextureFont::create(Font("Consolas", 20));
  now_ = bigBang();
  next_ = Universe(getWindowWidth(), getWindowHeight());
  sysinfo_.init(now_);
}

void ScreenSaver::draw() {
  gl::draw(now_.texture(), getWindowBounds());
  font_->drawString(sysinfo_.msg(), getWindowBounds());
}

void ScreenSaver::update() {
  if (sysinfo_.elapsed() >= REFRESH_TIME) {
    now_ = bigBang();
    sysinfo_.init(now_);
  } else {
    sysinfo_.onPreGen(now_);

    now_.next(next_);
    swap(now_, next_);

    sysinfo_.onPostGen(now_);
  }
}

Color ScreenSaver::color(const Cell &cell) const {
  Color yellow{ Color::hex(0xFFFF00) };
  return cell.isDead() ? Color::black() : yellow;
}

Universe ScreenSaver::bigBang() const {
  return Universe::bigBang(getWindowWidth(), getWindowHeight());
}

CINDER_APP_SCREENSAVER(ScreenSaver, RendererGl)
