#include "CellAnimation.h"

#include "CellRenderer.h"
#include "Slice.h"

#include "../Presenter/BoardLayout.h"
#include "../Presenter/Config.h"

using namespace std;

using namespace ci;

shared_ptr<Slice> getSlice(int value) {
  return CellRenderer::instance().render(value, BoardLayout::cellSize());
}

Sprite placePieceAnimation2(int value) {
  auto wait = Clip().duration(Config::ANIM_DURATION);
  Clip clip(getSlice(value));
  auto appear = clip.fadeby(1.f, 0.f).duration(Config::ANIM_DURATION);
  Animation anim = { wait, appear };
  return { anim.wrap(WrapMode::CLAMP_FOREVER) };
}

Sprite movePieceAnimation2(const Cell &src, const Cell &dst) {
  auto tex = getSlice(dst.value());
  auto offset = BoardLayout::distance(src.coord(), dst.coord());
  Animation anim = {
    Clip(tex).moveby(offset).duration(Config::ANIM_DURATION).reverse()
  };
  return { anim.wrap(WrapMode::CLAMP_FOREVER) };
}

Sprite promotionPieceAnimation2(int level) {
  auto tex = getSlice(level + 1);
  auto clip0 = Clip(tex).scale(1.f, Config::ENLARGE_RATIO).duration(
      Config::ANIM_DURATION);
  auto clip1 = Clip(tex).scale(Config::ENLARGE_RATIO, 1.f).duration(
      Config::ANIM_DURATION);
  Animation anim = { clip0, clip1 };
  return { anim.wrap(WrapMode::CLAMP_FOREVER) };
}

Sprite mergeAnimation2(const Sprite &sprite, const Cell &src, const Cell &dst) {
  auto anim = sprite.layer(sprite.layers() - 1);
  anim.wrap(WrapMode::CLAMP_FOREVER);
  return { { 1, movePieceAnimation2(src, dst).layer(0) +
                    promotionPieceAnimation2(dst.value()).layer(0) },
           { 0, anim } };
}

Sprite emptyCellAnimation2() {
  auto tex = getSlice(0);
  Animation anim = { Clip(tex).duration(Config::ANIM_DURATION) };
  return { anim.wrap(WrapMode::LOOP) };
}
