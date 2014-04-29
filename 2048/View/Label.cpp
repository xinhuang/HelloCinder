#include "Label.h"

using namespace std;

#include <cinder/Text.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;

struct Label::Data {
  string text;
  float fontSize = 60.f;
  Vec2f size;
  Vec2f location;
  Color foreColor = Color::white();
  Color backColor = Color::black();
};

Label::Label() : d(make_unique<Data>()) {}

Label::~Label() {}

void Label::setText(const std::string &text) { d->text = text; }

void Label::setForeColor(const Color &color) { d->foreColor = color; }

void Label::setBackColor(const Color &color) { d->backColor = color; }

void Label::setFontSize(float size) { d->fontSize = size; }

void Label::setSize(const ci::Vec2f &size) { d->size = size; }

void Label::setLocation(const ci::Vec2f &loc) { d->location = loc; }

const ci::Vec2f &Label::size() const { return d->size; }

const ci::Vec2f &Label::location() const { return d->location; }

void Label::draw() const {
  gl::enableAlphaBlending();

  gl::color(Color::white());
  auto font = Font("Arial", d->fontSize);
  auto tb = TextBox().alignment(TextBox::CENTER).font(font).text(d->text);
  tb.setSize(d->size);
  tb.setColor(d->foreColor);
  tb.setBackgroundColor(d->backColor);
  gl::draw(tb.render(), rect());

  gl::disableAlphaBlending();
}
