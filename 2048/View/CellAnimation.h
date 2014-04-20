#pragma once

#include "Animation.h"

#include "../Presenter/Cell.h"
#include "../Presenter/Environment.h"

ci::gl::TextureRef getTex(int value);
Animation placePieceAnimation(int value);
Animation movePieceAnimation(const Cell &src, const Cell &dst);
Animation promotionPieceAnimation(int level);
Animation mergeAnimation(const Cell &src, const Cell &dst);
Animation emptyCellAnimation();
