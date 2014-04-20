#include "CellAnimation.h"

using namespace std;

ci::gl::TextureRef getTex(int value) {
  return PieceRenderer::instance().render(value,
                                          Environment::instance().cellSize());
}

Animation emptyCellAnimation() {
  return Animation{ make_unique<NullRenderer>() };
}

Animation placePieceAnimation(int value) {
  auto tex = getTex(value);
  return scaleBy(tex, 0.5f, 1.03f, 3) + scaleBy(tex, 1.03f, 1.f, 3);
}

Animation movePieceAnimation(const Cell &src, const Cell &dst) {
  auto tex = getTex(dst.value());
  auto delta = Environment::instance().distance(src.pos(), dst.pos());
  return moveBy(tex, delta, 3).reverse();
}

Animation promotionPieceAnimation(int level) {
  auto tex = getTex(level * 2);
  return scaleBy(tex, 1.f, 1.03f, 3) + scaleBy(tex, 1.03f, 1.f, 3);
}

Animation mergeAnimation(const Cell &src, const Cell &dst) {
  return movePieceAnimation(src, dst) + promotionPieceAnimation(dst.value());
}
