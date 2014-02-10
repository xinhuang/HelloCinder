#include "stdafx.h"
#include "ClockApp.h"

#include "resource.h"

#include <cinder/ImageIo.h>

void ClockApp::prepareSettings(Settings *settings) {
  settings->setBorderless();
  settings->setAlwaysOnTop();
  settings->setWindowSize(68, 20);
  settings->setFrameRate(30);
}

void ClockApp::setup() {
  _font = TextureFont::create(Font("Consolas", 20));
}

void ClockApp::draw() {
  gl::clear(Color::black());
  gl::color(Color::white());
  Rectf windowRect{ 0, 0, (float)getWindowWidth(), (float)getWindowHeight() };

  _font->drawString(currentTime(), windowRect);
}

CINDER_APP_BASIC(ClockApp, RendererGl)
