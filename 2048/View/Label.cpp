#include "Label.h"

#include "Slice.h"

#include <cassert>

using namespace std;

#include <cinder/Text.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/gl/Fbo.h>

using namespace ci;

struct Label::Data {
  float cornerRadius = 0.f;
  ColorA foreColor = Color::white();
  ColorA backColor = Color::black();
  string text;
  Font font;
  mutable gl::TextureRef clip;
};

Label::Label() : d(make_unique<Data>()) {}

Label::~Label() {}

void Label::setForeColor(const ColorA &color) {
  d->foreColor = color;
  d->clip.reset();
}

void Label::setBackColor(const ColorA &color) {
  d->backColor = color;
  d->clip.reset();
}

const ci::ColorA& Label::backColor() const {
  return d->backColor;
}

void Label::setFont(const Font& font) {
  d->font = font;
  d->clip.reset();
}

void Label::setSize(const ci::Vec2f &size) {
  Control::setSize(size);
  d->clip.reset();
}

void Label::setCornerRadius(float value) {
  d->cornerRadius = value;
  d->clip.reset();
}

const Font &Label::font() const { return d->font; }

void Label::setText(const std::string &text) {
  d->text = text;
  d->clip.reset();
}

void Label::updateCache() const {
  auto tb = TextBox().alignment(TextBox::CENTER).font(d->font).text(d->text);
  tb.setSize(size());
  tb.setColor(d->foreColor);
  tb.setBackgroundColor(ColorA::hexA(0x00000000));

  auto tex = tb.render();

  gl::Fbo fbo((int)(size().x), (int)(size().y), true);
  fbo.bindFramebuffer();
  gl::setViewport({ { 0, 0 }, size() });
  gl::setMatricesWindow(size(), false);
  gl::enableAlphaBlending();
  gl::color(d->backColor);
  gl::drawSolidRoundedRect({ { 0.f, 0.f }, size() }, d->cornerRadius);
  gl::color(Color::white());
  gl::draw(tex);
  gl::disableAlphaBlending();
  fbo.unbindFramebuffer();

  d->clip = gl::Texture::create(fbo.getTexture());
}

void Label::draw() {
  if (!d->clip)
    updateCache();

  gl::enableAlphaBlending();

  gl::color(Color::white());
  gl::draw(d->clip, screenRect());

  gl::disableAlphaBlending();
}
