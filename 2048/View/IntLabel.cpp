#include "IntLabel.h"

#include "Slice.h"
#include "Sprite.h"

#include <cassert>

using namespace std;

#include <cinder/Text.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;

struct IntLabel::Data {
  bool dirty = true;
  Color foreColor = Color::white();
  Color backColor = Color::black();
  int value;
  Font font;
  mutable Sprite sprite;
};

IntLabel::IntLabel() : d(make_unique<Data>()) {}

IntLabel::~IntLabel() {}

void IntLabel::setForeColor(const Color &color) {
  d->foreColor = color;
  d->dirty = true;
}

void IntLabel::setBackColor(const Color &color) {
  d->backColor = color;
  d->dirty = true;
}

const ci::Color &IntLabel::backColor() const { return d->backColor; }

void IntLabel::setFont(const Font &font) {
  d->font = font;
  d->dirty = true;
}

void IntLabel::setSize(const ci::Vec2f &size) {
  Control::setSize(size);
  d->dirty = true;
}

const Font &IntLabel::font() const { return d->font; }

void IntLabel::setValue(int value) {
  if (value == d->value)
    return;
  d->value = value;
  d->dirty = true;
}

void IntLabel::updateCache() const {
  auto tb = TextBox().alignment(TextBox::CENTER).font(d->font).text(
      to_string(d->value));
  tb.setSize(size());
  tb.setColor(d->foreColor);
  tb.setBackgroundColor(d->backColor);

  auto tex = gl::Texture::create(tb.render());
  auto anim = Animation{ tex };
  d->sprite = Sprite(anim.wrap(WrapMode::CLAMP_FOREVER));
  d->sprite.setDepth(10);
  d->sprite.setRect(screenRect());
  d->sprite.setName("IntLabel");

  d->dirty = false;
}

void IntLabel::update() {
  Control::update();
  if (d->dirty)
    updateCache();
}
