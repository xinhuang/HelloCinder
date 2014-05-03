#include "CellRenderer.h"

#include "../Presenter/Config.h"

#include <cinder/gl/Fbo.h>
#include <cinder/Text.h>
#include <cinder/Font.h>

using namespace ci;

#include <unordered_map>
#include <cassert>

using namespace std;

struct CellRenderer::Data {
  gl::Fbo fbo;
  Font font;
  unordered_map<int, shared_ptr<Slice> > slices;

  const shared_ptr<Slice> &newTexture(int value, const ci::Vec2i &size) {
    if (!fbo || fbo.getWidth() != size.x || fbo.getHeight() != size.y)
      fbo = gl::Fbo(size.x, size.y, true);

    fbo.bindFramebuffer();
    gl::setViewport({ 0, 0, size.x, size.y });
    gl::setMatricesWindow(size, false);
    gl::enableAlphaBlending();
    gl::clear(ColorA(1, 1, 1, 0));

    assert(value >= 0);
    size_t color_index = value;
    if (color_index >=
        sizeof(Config::CELL_COLORS) / sizeof(Config::CELL_COLORS[0]))
      color_index = sizeof(Config::CELL_COLORS) / Config::CELL_COLORS[0] - 1;
    auto bk_color = Config::CELL_COLORS[color_index];
    auto fore_color = Config::FORE_COLORS[color_index];

    gl::color(Color::hex(bk_color));
    Rectf rect = { 0.f, 0.f, (float)size.x, (float)size.y };
    gl::drawSolidRoundedRect(rect, 5.f);

    if (value > 0) {
      gl::color(Color::white());
      TextBox tbox =
          TextBox().alignment(TextBox::CENTER).size(size).font(font).text(
              to_string(1 << value));
      tbox.setColor(Color::hex(fore_color));
      tbox.setBackgroundColor(ColorA(1, 1, 1, 0));
      gl::draw(tbox.render());
    }
    gl::disableAlphaBlending();
    fbo.unbindFramebuffer();
    auto tex = gl::Texture::create(fbo.getTexture());
    slices[value] = make_shared<Slice>(tex);
    return slices[value];
  }
};

CellRenderer::CellRenderer() : d(make_unique<Data>()) {
  d->font = Font(Config::FONT, Config::FONT_WEIGHT);
}

CellRenderer::~CellRenderer() {}

shared_ptr<Slice> CellRenderer::render(int value, const ci::Vec2f &size) {
  shared_ptr<Slice> slice;
  auto iter = d->slices.find(value);
  if (iter == d->slices.end())
    slice = d->newTexture(value, size);
  else
    slice = iter->second;
  return slice;
}
