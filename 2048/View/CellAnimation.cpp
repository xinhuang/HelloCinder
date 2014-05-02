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

//Animation emptyCellAnimation() {
//  auto tex = getTex(0);
//  return Animation{ make_unique<TextureRenderer>(tex) };
//}
//
//Animation placePieceAnimation(int value) {
//  auto tex = getTex(value);
//  return emptyCellAnimation() + fade(tex, 0.f, 1.f, Config::ANIM_FRAMES);
//}
//
//Animation movePieceAnimation(const Cell &src, const Cell &dst) {
//  auto tex = getTex(dst.value());
//  auto delta = BoardLayout::distance(src.coord(), dst.coord());
//  return moveBy(tex, delta, Config::ANIM_FRAMES).reverse();
//}
//
//Animation promotionPieceAnimation(int level) {
//  auto tex = getTex(level + 1);
//  return scaleBy(tex, 1.f, Config::ENLARGE_RATIO, Config::ANIM_FRAMES) +
//         scaleBy(tex, Config::ENLARGE_RATIO, 1.f, Config::ANIM_FRAMES);
//}
//
//Animation mergeAnimation(const Cell &src, const Cell &dst) {
//  return movePieceAnimation(src, dst) + promotionPieceAnimation(dst.value());
//}

// ------------------------------------------------------ //

Sprite placePieceAnimation2(int value) {
  auto tex = getTex(value);
  auto wait = Clip().duration(0.3f);
  auto appear = Clip(tex).fadeby(1.f, 0.f).duration(0.3f);
  Animation anim = { wait, appear };
  return { { 0, anim } };
}

Sprite movePieceAnimation2(const Cell &src, const Cell &dst) {
  auto tex = getTex(dst.value());
  auto offset = BoardLayout::distance(dst.coord(), src.coord());
  Animation anim = { Clip(tex).moveby(offset).duration(0.3f) };
  return { { 0, anim } };
}

Sprite promotionPieceAnimation2(int level) {
  auto tex = getTex(level + 1);
  Animation anim = {
    Clip(tex).scaleby(Config::ENLARGE_RATIO).duration(0.3f),
    Clip(tex).scaleby(1.f, Config::ENLARGE_RATIO).duration(0.3f)
  };
  return{ { 0, anim } };
}

Sprite mergeAnimation2(const Cell &src, const Cell &dst) {
  return { { 0, movePieceAnimation2(src, dst).layer(0) +
                    promotionPieceAnimation2(dst.value()).layer(0) } };
}

Sprite emptyCellAnimation2() {
  auto tex = getTex(0);
  Animation anim = { Clip(tex).duration(0.3f) };
  return { { 0, anim.cyclic() } };
}
