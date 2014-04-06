#include "PieceRenderer.h"

#include "Piece.h"
#include "Config.h"

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
    gl::color(Color::hex(Config::BK_COLOR));
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

void PieceRenderer::draw(const Piece& p, Rectf rect) {
  const auto& size = rect.getSize();
  gl::TextureRef tex;

  auto iter = d->texs.find(p.value);
  if (iter == d->texs.end())
    tex = d->newTexture(p.value, size);
  else
    tex = iter->second;

  if (p.state == PieceState::Spawn) {
    Vec2i newSize = size / 4 * (4 - p.count);
    Vec2i newPos = rect.getUpperLeft() + (rect.getSize() - newSize) / 2;
    rect = { newPos, newPos + newSize };
    p.count--;
    if (p.count == 0)
      p.state = PieceState::Still;
  }
  gl::color(Color::white()); 
  gl::draw(tex, rect);
}

void PieceRenderer::draw(const int value, ci::Rectf rect) {
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

ci::gl::TextureRef PieceRenderer::render(const int value, const ci::Vec2f& size) {
  gl::TextureRef tex;
  auto iter = d->texs.find(value);
  if (iter == d->texs.end())
    tex = d->newTexture(value, size);
  else
    tex = iter->second;
  return tex;
}
