#pragma once

#include "Sprite.h"

class Cell;

animation::Sprite createPlacePieceSprite(int value);
animation::Sprite createMovePieceSprite(const Cell &src, const Cell &dst);
animation::Sprite createPromotionPieceSprite(int level);
animation::Sprite createMergeSprite(const animation::Sprite &sprite, const Cell &src, const Cell &dst);
animation::Sprite createEmptyCellSprite();
