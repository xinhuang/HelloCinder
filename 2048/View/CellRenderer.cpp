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
  unordered_map<int, gl::TextureRef> texs;
  
  gl::TextureRef& newTexture(int value, const ci::Vec2i& size) {
    if (!fbo || fbo.getWidth() != size.x || fbo.getHeight() != size.y)
      fbo = gl::Fbo(size.x, size.y, true);

    fbo.bindFramebuffer();
    gl::setViewport({ 0, 0, size.x, size.y });
    gl::setMatricesWindow(size, false);
    gl::enableAlphaBlending();
    gl::clear(ColorA(1, 1, 1, 0));

    assert(value >= 0);
    size_t color_index = value;
    if (color_index >= sizeof(Config::CELL_COLORS) / sizeof(Config::CELL_COLORS[0]))
      color_index = sizeof(Config::CELL_COLORS) / Config::CELL_COLORS[0] - 1;
    auto bk_color = Config::CELL_COLORS[color_index];
    auto fore_color = Config::FORE_COLORS[color_index];

    gl::color(Color::hex(bk_color));
    Rectf rect = { 0.f, 0.f, (float)size.x, (float)size.y };
    gl::drawSolidRoundedRect(rect, 5.f);

    if (value > 0) {
      TextBox tbox =
        TextBox().alignment(TextBox::CENTER).size(size).font(font).text(
        to_string(1 << value));
      tbox.setColor(Color::hex(fore_color));
      tbox.setBackgroundColor(ColorA(1, 1, 1, 0));
      gl::draw(tbox.render());
    }
    gl::disableAlphaBlending();
    fbo.unbindFramebuffer();
    auto tex = fbo.getTexture();
    texs[value] = gl::Texture::create(tex);
    return texs[value];
  }
};

CellRenderer::CellRenderer() : d(make_unique<Data>()) {
  d->font = Font("Arial", 60);
}

CellRenderer::~CellRenderer() {}

void CellRenderer::draw(const int value, ci::Rectf rect) {
  const auto& size = rect.getSize();
  gl::TextureRef tex;

  auto iter = d->texs.find(value);
  if (iter == d->texs.end())
    tex = d->newTexture(value, size);
  else
    tex = iter->second;

  gl::color(Color::white());
  gl::draw(tex, rect);
}

ci::gl::TextureRef CellRenderer::render(const int value, const ci::Vec2f& size) {
  gl::TextureRef tex;
  auto iter = d->texs.find(value);
  if (iter == d->texs.end())
    tex = d->newTexture(value, size);
  else
    tex = iter->second;
  return tex;
}
