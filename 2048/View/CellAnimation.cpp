#include "CellAnimation.h"

#include "CellRenderer.h"

#include "../Presenter/BoardLayout.h"
#include "../Presenter/Config.h"

using namespace std;

ci::gl::TextureRef getTex(int value) {
  return CellRenderer::instance().render(value, BoardLayout::cellSize());
}

Animation emptyCellAnimation() {
  auto tex = getTex(0);
  return Animation{ make_unique<TextureRenderer>(tex) };
}

Animation placePieceAnimation(int value) {
  auto tex = getTex(value);
  return emptyCellAnimation() + scaleBy(tex, 0.5f, 1.03f, Config::ANIM_FRAMES) + scaleBy(tex, 1.03f, 1.f, Config::ANIM_FRAMES);
}

Animation movePieceAnimation(const Cell &src, const Cell &dst) {
  auto tex = getTex(dst.value());
  auto delta = BoardLayout::distance(src.coord(), dst.coord());
  return moveBy(tex, delta, Config::ANIM_FRAMES).reverse();
}

Animation promotionPieceAnimation(int level) {
  auto tex = getTex(level + 1);
  return scaleBy(tex, 1.f, 1.03f, Config::ANIM_FRAMES) + scaleBy(tex, 1.03f, 1.f, Config::ANIM_FRAMES);
}

Animation mergeAnimation(const Cell &src, const Cell &dst) {
  return movePieceAnimation(src, dst) + promotionPieceAnimation(dst.value());
}
