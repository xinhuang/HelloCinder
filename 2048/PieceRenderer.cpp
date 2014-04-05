#include "PieceRenderer.h"

#include "Piece.h"

#include <cinder/Text.h>
#include <cinder/Font.h>

using namespace ci;

using namespace std;

struct PieceRenderer::Data {
  gl::Fbo fbo;
  Font font;
};

PieceRenderer::PieceRenderer() : d(make_unique<Data>()) {
  d->font = Font("Arial", 60);
}

PieceRenderer::~PieceRenderer() {}

gl::Texture &PieceRenderer::render(const Piece &p, const ci::Vec2i &size) {
  if (!d->fbo || d->fbo.getWidth() != size.x || d->fbo.getHeight() != size.y)
    d->fbo = gl::Fbo(size.x, size.y, true);

  d->fbo.bindFramebuffer();
  gl::setViewport({ 0, 0, size.x, size.y });
  gl::setMatricesWindow(size, false);
  gl::clear(Color::hex(0xFFFFFFFF));
  Rectf pieceRect = { 0.f, 0.f, (float)size.x, (float)size.y };
  gl::color(Color::hex(0xDDDDDD00));
  gl::drawSolidRoundedRect(pieceRect, 5.f);
  TextBox tbox =
      TextBox().alignment(TextBox::CENTER).size(size).font(d->font).text(
          to_string(p.value));
  tbox.setColor(Color(1.0f, 0.65f, 0.35f));
  tbox.setBackgroundColor(ColorA(1, 1, 1, 0));
  gl::enableAlphaBlending();
  gl::draw(tbox.render());
  gl::disableAlphaBlending();
  d->fbo.unbindFramebuffer();
  return d->fbo.getTexture();
}