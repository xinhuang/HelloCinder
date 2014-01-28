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
  gl::TextureRef _digitTexture =
      gl::Texture::create(loadImage(loadResource(IDB_DIGITS, "JPG")));
  _sprites['0'] = SpriteSheet::create(
    _digitTexture, Area(40 + 120 * 0, 50, 40 + 120 * 1, 40 + 220));
  _sprites['1'] = SpriteSheet::create(
    _digitTexture, Area(40 + 120 * 1, 50, 40 + 120 * 2, 40 + 220));
  _sprites['2'] = SpriteSheet::create(
    _digitTexture, Area(40 + 120 * 2, 50, 40 + 120 * 3, 40 + 220));
}

void ClockApp::draw() {
  gl::clear(Color::white());
  Rectf windowRect{ 0, 0, (float)getWindowWidth(), (float)getWindowHeight() };

  //_font->drawString(currentTime(), windowRect);

  _sprites['0']->draw(Rectf(8 * 0, 0, 8 * 1, 20));
  _sprites['1']->draw(Rectf(8 * 1, 0, 8 * 2, 20));
  _sprites['2']->draw(Rectf(8 * 2, 0, 8 * 3, 20));

  bool a = getWindow()->isAlwaysOnTop();
  a = false;
}

CINDER_APP_BASIC(ClockApp, RendererGl)
