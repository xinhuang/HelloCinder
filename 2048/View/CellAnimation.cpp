#include "CellAnimation.h"

#include "CellRenderer.h"
#include "TextureRenderer.h"
#include "NullRenderer.h"

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
  return emptyCellAnimation() +
    fade(tex, 0.f, 1.f, Config::ANIM_FRAMES);
}

Animation movePieceAnimation(const Cell &src, const Cell &dst) {
  auto tex = getTex(dst.value());
  auto delta = BoardLayout::distance(src.coord(), dst.coord());
  return moveBy(tex, delta, Config::ANIM_FRAMES).reverse();
}

Animation promotionPieceAnimation(int level) {
  auto tex = getTex(level + 1);
  return scaleBy(tex, 1.f, Config::ENLARGE_RATIO, Config::ANIM_FRAMES) +
         scaleBy(tex, Config::ENLARGE_RATIO, 1.f, Config::ANIM_FRAMES);
}

Animation mergeAnimation(const Cell &src, const Cell &dst) {
  return movePieceAnimation(src, dst) + promotionPieceAnimation(dst.value());
}

Animation2 placePieceAnimation2(int value) {
  auto tex = getTex(value);
  return { Clip().duration(0.3f), Clip(tex).fadeby(1.f, 0.f).duration(0.3f) };
}

Animation2 movePieceAnimation2(const Cell &src, const Cell &dst);
Animation2 promotionPieceAnimation2(int level);
Animation2 mergeAnimation2(const Cell &src, const Cell &dst);

Animation2 emptyCellAnimation2() {
  return { Clip().duration(0.3f) };
}
