#include "stdafx.h"
#include "TextBox.h"

#include <cinder/gl/TextureFont.h>
using namespace ci;
using namespace ci::gl;

using namespace std;

namespace hcui {

struct TextBox::Impl {
  Impl() {
    font_ = TextureFont::create(Font("Consolas", 20));
  }

  string text_;
  TextureFontRef font_;
};

TextBox::TextBox(int x, int y) : m(make_unique<Impl>()) {}

void TextBox::set_Text(string text) { m->text_ = move(text); }

void TextBox::do_events() {
  gl::color(Color::white());
  Rectf windowRect{ 10, 10, 400.f, 300.f };

  m->font_->drawString(m->text_, windowRect);
}

}
