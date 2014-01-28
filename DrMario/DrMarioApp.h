#pragma once

#include <cinder/cinder.h>
#include <cinder/app/AppBasic.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

#include "Pill.h"

class DrMarioApp : public AppBasic {
  const int MaxRow = 20;
  const int MaxCol = 20;

  const Color Red = Color::hex(0xFF0000);
  const Color Green = Color::hex(0x00FF00);
  const Color Blue = Color::hex(0x0000FF);
  const Color Black = Color::black();

  bool over_;
  vector<Pill> pills_;

public:
  DrMarioApp() : over_(false) {}
  void draw() override;
  void update() override;

private:
  bool tryErase();

  Pill *pillAt(Vec2f loc);
  Pill *pillAt(float x, float y);
  const Color &colorAt(float x, float y);
  bool canDrop(const Pill &pill) const;
  void eraseVertical(float x, float y, tuple<int, int> range);
  void eraseHorizontal(float x, float y, tuple<int, int> range);

  Vec2i dropPortal() const {
    return { 5, 5 };
  }
};
