#include "Label.h"

using namespace std;

#include <cinder/Text.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;

struct Label::Data {
  string text;
  float fontSize = 60.f;
  Color foreColor = Color::white();
  Color backColor = Color::black();
};

Label::Label() : d(make_unique<Data>()) {}

Label::~Label() {}

void Label::setText(const std::string& text) {
  d->text = text;
}

void Label::setForeColor(const Color& color) {
  d->foreColor = color;
}

void Label::setBackColor(const Color& color) {
  d->backColor = color;
}

void Label::setFontSize(float size) {
  d->fontSize = size;
}

void Label::draw(const ci::Rectf& rect) const {
  gl::enableAlphaBlending();

  gl::color(Color::white());
  auto font = Font("Arial", d->fontSize);
  auto tb = TextBox().alignment(TextBox::LEFT).font(font).text(d->text);
  tb.setSize(rect.getSize());
  tb.setColor(d->foreColor);
  tb.setBackgroundColor(d->backColor);
  gl::draw(tb.render(), rect);

  gl::disableAlphaBlending();
}
