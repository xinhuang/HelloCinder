#include "GameOverWindow.h"

using namespace std;

#include <cinder/gl/gl.h>
#include <cinder/Color.h>
#include <cinder/Text.h>
#include <cinder/gl/Texture.h>

using namespace ci;

using namespace ui;

struct GameOverWindow::Data {

};

GameOverWindow::GameOverWindow() : d(make_unique<Data>()) {
}

GameOverWindow::~GameOverWindow() {}

void GameOverWindow::draw() {
  Control::draw();

  gl::enableAlphaBlending();

  const auto& rect = this->rect();
  gl::color(ColorA(1.f, 1.f, 1.f, 0.6f));
  gl::drawSolidRect(rect);

  gl::color(Color::white());
  auto font = Font("Arial", 100.f);
  auto tb = TextBox().alignment(TextBox::CENTER).font(font).text("Game over!");
  tb.setSize(rect.getSize());
  tb.setColor(Color::hex(0x776E65));
  gl::draw(tb.render(), rect);

  gl::disableAlphaBlending();
}
