#include "CellAnimation.h"
#include "../Presenter/Scene.h"

using namespace std;

ci::gl::TextureRef getTex(int value) {
  return PieceRenderer::instance().render(value,
                                          Scene::cellSize());
}

Animation emptyCellAnimation() {
  auto tex = getTex(0);
  return Animation{ make_unique<TextureRenderer>(tex) };
}

Animation placePieceAnimation(int value) {
  auto tex = getTex(value);
  return scaleBy(tex, 0.5f, 1.03f, 3) + scaleBy(tex, 1.03f, 1.f, 3);
}

Animation movePieceAnimation(const Cell &src, const Cell &dst) {
  auto tex = getTex(dst.value());
  auto delta = Scene::distance(src.coord(), dst.coord());
  return moveBy(tex, delta, 3).reverse();
}

Animation promotionPieceAnimation(int level) {
  auto tex = getTex(level + 1);
  return scaleBy(tex, 1.f, 1.03f, 3) + scaleBy(tex, 1.03f, 1.f, 3);
}

Animation mergeAnimation(const Cell &src, const Cell &dst) {
  return movePieceAnimation(src, dst) + promotionPieceAnimation(dst.value());
}
