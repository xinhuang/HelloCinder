#pragma once

#include <stdint.h>

namespace Config {
const float BORDER_WIDTH = 10.f;
const int SIZE = 4;
const float HEADER_HEIGHT = 100.f;

const uint32_t CELL_COLORS[] = { 0xCDC0B4, 0xEEE4DA, 0xEDE0C8, 0xf2b179,
                                 0xf59563, 0xf67c5f, 0xf65e3b, 0xedcf72,
                                 0xedcc61, 0xedc850, 0xedc53f, 0xedc22e, };

const uint32_t FORE_COLORS[] = { 0x000000, 0x776E65, 0x776E65, 0xf9f6f2,
                                 0xf9f6f2, 0xf9f6f2, 0xf9f6f2, 0xf9f6f2,
                                 0xf9f6f2, 0xf9f6f2, 0xf9f6f2, 0xf9f6f2, };

const uint32_t BOARD_COLOR = 0xBBADA0;
const uint32_t BK_COLOR = 0xFAF8EF;

const float FRAME_RATE = 30.f;
const float ANIM_DURATION = 0.1f;
const float EFFECT_DURATION = 0.5f;
const float ENLARGE_RATIO = 1.2f;

const char FONT[] = "Arial";
const float FONT_WEIGHT = 50;

const uint32_t LABEL_FORE_COLOR = 0xEEE4DA;
const uint32_t SCORE_COLOR = 0xFFFFFF;
}
