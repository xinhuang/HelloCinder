#pragma once

#include "Sprite.h"

class Cell;

Sprite createPlacePieceSprite(int value);
Sprite createMovePieceSprite(const Cell &src, const Cell &dst);
Sprite createPromotionPieceSprite(int level);
Sprite createMergeSprite(const Sprite &sprite, const Cell &src, const Cell &dst);
Sprite createEmptyCellSprite();
