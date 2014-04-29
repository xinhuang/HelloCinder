#include "Label.h"

#include "IRenderable.h"

#include <cassert>

using namespace std;

#include <cinder/Text.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;

struct Label::Data {
  Font font;
  Vec2f size;
  Vec2f location;
  Color foreColor = Color::white();
  Color backColor = Color::black();
  string text;
  mutable unique_ptr<IRenderable> render;
};

Label::Label() : d(make_unique<Data>()) {}

Label::~Label() {}

void Label::setForeColor(const Color &color) {
  d->foreColor = color;
  d->render.reset();
}

void Label::setBackColor(const Color &color) {
  d->backColor = color;
  d->render.reset();
}

void Label::setFont(const Font& font) {
  d->font = font;
  d->render.reset();
}

void Label::setSize(const ci::Vec2f &size) {
  d->size = size;
  d->render.reset();
}

void Label::setLocation(const ci::Vec2f &loc) { d->location = loc; }

const ci::Vec2f &Label::size() const { return d->size; }

const ci::Vec2f &Label::location() const { return d->location; }

const Font &Label::font() const { return d->font; }

void Label::setText(const std::string &text) {
  d->text = text;
  d->render.reset();
}

void Label::updateCache() const {
  auto tb = TextBox().alignment(TextBox::CENTER).font(d->font).text(d->text);
  tb.setSize(d->size);
  tb.setColor(d->foreColor);
  tb.setBackgroundColor(d->backColor);

  d->render.reset(new TextureRenderer(gl::Texture::create(tb.render())));
}

void Label::draw() const {
  if (!d->render)
    updateCache();

  gl::enableAlphaBlending();

  gl::color(Color::white());
  d->render->draw(rect());

  gl::disableAlphaBlending();
}
