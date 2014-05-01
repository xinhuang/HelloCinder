#pragma once

#include "Animation.h"

#include "../Presenter/Cell.h"

Animation placePieceAnimation(int value);
Animation movePieceAnimation(const Cell &src, const Cell &dst);
Animation promotionPieceAnimation(int level);
Animation mergeAnimation(const Cell &src, const Cell &dst);
Animation emptyCellAnimation();

Animation2 placePieceAnimation2(int value);
Animation2 movePieceAnimation2(const Cell &src, const Cell &dst);
Animation2 promotionPieceAnimation2(int level);
Animation2 mergeAnimation2(const Cell &src, const Cell &dst);
Animation2 emptyCellAnimation2();
