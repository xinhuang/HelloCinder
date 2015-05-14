#include "stdafx.h"
#include "ClockApp.h"

#include "resource.h"

#include <cinder/ImageIo.h>

void ClockApp::prepareSettings(Settings *settings) {
  settings->setBorderless();
  settings->setAlwaysOnTop();
  settings->setWindowSize(68, 20);
  settings->setFrameRate(12);
}

void ClockApp::setup() {
  _font = TextureFont::create(Font("Consolas", 20));

  auto screenSize = Display::getMainDisplay()->getBounds().getSize();
  auto pos = screenSize - getWindowSize() - Vec2i{30, 30};
  setWindowPos(pos);
}

void ClockApp::draw() {
  gl::enableAlphaBlending();
  gl::clear(Color::black());
  gl::color(Color::white());
  Rectf windowRect{ 1, 1, (float)getWindowWidth(), (float)getWindowHeight() };

  _font->drawString(currentTime(), windowRect);
}

CINDER_APP_BASIC(ClockApp, RendererGl)
