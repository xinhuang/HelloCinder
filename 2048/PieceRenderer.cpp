#include "PieceRenderer.h"

#include "Piece.h"

#include <cinder/Text.h>
#include <cinder/Font.h>

using namespace ci;

#include <unordered_map>

using namespace std;

struct PieceRenderer::Data {
  gl::Fbo fbo;
  Font font;
  unordered_map<int, gl::TextureRef> texs;
  
  gl::TextureRef& newTexture(int value, const ci::Vec2i& size) {
    if (!fbo || fbo.getWidth() != size.x || fbo.getHeight() != size.y)
      fbo = gl::Fbo(size.x, size.y, true);

    fbo.bindFramebuffer();
    gl::setViewport({ 0, 0, size.x, size.y });
    gl::setMatricesWindow(size, false);
    gl::clear(Color::hex(0xFFFFFFFF));
    gl::color(Color::hex(0xDDDDDD00));
    Rectf rect = { 0.f, 0.f, (float)size.x, (float)size.y };
    gl::drawSolidRoundedRect(rect, 5.f);
    TextBox tbox =
      TextBox().alignment(TextBox::CENTER).size(size).font(font).text(
      to_string(value));
    tbox.setColor(Color(1.0f, 0.65f, 0.35f));
    tbox.setBackgroundColor(ColorA(1, 1, 1, 0));
    gl::enableAlphaBlending();
    gl::draw(tbox.render());
    gl::disableAlphaBlending();
    fbo.unbindFramebuffer();
    auto tex = fbo.getTexture();
    texs[value] = gl::Texture::create(tex);
    return texs[value];
  }
};

PieceRenderer::PieceRenderer() : d(make_unique<Data>()) {
  d->font = Font("Arial", 60);
}

PieceRenderer::~PieceRenderer() {}

gl::Texture &PieceRenderer::render(const Piece &p, const ci::Vec2i &size) {
  gl::TextureRef tex;

  auto iter = d->texs.find(p.value);
  if (iter == d->texs.end())
    tex = d->newTexture(p.value, size);
  else
    tex = iter->second;

  if (p.state == PieceState::Spawn) {
    Vec2i newSize = size / 4 * (4 - p.count);
    Rectf rect((size - newSize) / 2.f, newSize);
    d->fbo.bindFramebuffer();
    gl::setViewport({ 0, 0, size.x, size.y });
    gl::setMatricesWindow(size, false);
    gl::clear(Color::hex(0xFFFFFFFF));
    gl::color(Color::white());
    gl::draw(tex, rect);
    d->fbo.unbindFramebuffer();
    p.count--;
    if (p.count == 0)
      p.state = PieceState::Still;
    return d->fbo.getTexture();
  }
  return *tex;
}