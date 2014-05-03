#include "CellAnimation.h"

#include "CellRenderer.h"
#include "Slice.h"

#include "../Presenter/BoardLayout.h"
#include "../Presenter/Config.h"

using namespace std;

using namespace ci;

ci::gl::TextureRef getTex(int value) {
  return CellRenderer::instance().render(value, BoardLayout::cellSize());
}

Sprite placePieceAnimation2(int value) {
  auto tex = getTex(value);
  auto wait = Clip().duration(0.3f);
  auto appear = Clip(tex).fadeby(1.f, 0.f).duration(0.3f);
  Animation anim = { wait, appear };
  return { { 0, anim.wrap(WrapMode::CLAMP_FOREVER) } };
}

Sprite movePieceAnimation2(const Cell &src, const Cell &dst) {
  auto tex = getTex(dst.value());
  auto offset = BoardLayout::distance(src.coord(), dst.coord());
  Animation anim = { Clip(tex).moveby(offset).duration(0.3f).reverse(), Clip(tex) };
  return { { 0, anim.wrap(WrapMode::CLAMP_FOREVER) } };
}

Sprite promotionPieceAnimation2(int level) {
  auto tex = getTex(level + 1);
  auto clip0 = Clip(tex).scale(1.f, Config::ENLARGE_RATIO).duration(0.3f);
  auto clip1 = Clip(tex).scale(Config::ENLARGE_RATIO, 1.f).duration(0.3f);
  Animation anim = { clip0, clip1 };
  return{ { 0, anim.wrap(WrapMode::CLAMP_FOREVER) } };
}

Sprite mergeAnimation2(const Cell &src, const Cell &dst) {
  return { { 0, movePieceAnimation2(src, dst).layer(0) +
                    promotionPieceAnimation2(dst.value()).layer(0) } };
}

Sprite emptyCellAnimation2() {
  auto tex = getTex(0);
  Animation anim = { Clip(tex).duration(0.3f) };
  return { { 0, anim.wrap(WrapMode::LOOP) } };
}
