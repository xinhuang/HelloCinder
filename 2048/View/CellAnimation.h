#pragma once

#include "Animation.h"

#include "../Presenter/Cell.h"

Animation placePieceAnimation(int value);
Animation movePieceAnimation(const Cell &src, const Cell &dst);
Animation promotionPieceAnimation(int level);
Animation mergeAnimation(const Cell &src, const Cell &dst);
Animation emptyCellAnimation();

Sprite placePieceAnimation2(int value);
Sprite movePieceAnimation2(const Cell &src, const Cell &dst);
Sprite promotionPieceAnimation2(int level);
Sprite mergeAnimation2(const Cell &src, const Cell &dst);
Sprite emptyCellAnimation2();
