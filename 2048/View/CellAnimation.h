#pragma once

#include "Sprite.h"

#include "../Presenter/Cell.h"

Sprite placePieceAnimation2(int value);
Sprite movePieceAnimation2(const Cell &src, const Cell &dst);
Sprite promotionPieceAnimation2(int level);
Sprite mergeAnimation2(const Cell &src, const Cell &dst);
Sprite emptyCellAnimation2();
