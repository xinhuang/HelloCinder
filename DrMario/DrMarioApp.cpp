#include "stdafx.h"
#include "DrMarioApp.h"

void DrMarioApp::draw() {
  gl::clear();

  gl::pushMatrices();
  gl::translate(Vec2f{ 200, 80 });
  for (auto &pill : pills_)
    pill.draw();
  gl::popMatrices();
}

void DrMarioApp::update() {
  bool dropped = false;
  for (auto &pill : pills_) {
    if (canDrop(pill)) {
      pill.drop();
      dropped = true;
    }
  }
  if (dropped)
    return;

  if (tryErase())
    return;

  pills_.emplace_back(dropPortal(), Blue, Red);
  if (!canDrop(pills_.back()))
    over_ = true;
}

vector<vector<Color> > matrix(int nrow, int ncol) {
  return vector<vector<Color> >(nrow, vector<Color>(ncol));
}

bool DrMarioApp::tryErase() {
  auto snapshot = matrix(MaxCol, MaxRow);
  for (const auto &p : pills_)
    p.fill(snapshot);

  vector<Vec2i> erasures;
  for (int col = 0; col < MaxCol; ++col) {
    int n = 0;
    int row = 0;
    while (row < MaxRow && snapshot[col][row] != Color())
      ++row;
    Color c;
    while (row < MaxRow) {
      if (c != snapshot[col][row]) {
        c = snapshot[col][row];
        n = 1;
      } else if (c != Color()) {
        ++n;
        const int KillLimit = 4;
        if (n >= KillLimit) {
          for (int i = 0; i < n; ++i)
            erasures.emplace_back(row - i, col);
        }
      }
      ++row;
    }
  }

  for (int row = 0; row < MaxRow; ++row) {
    int n = 0;
    int col = 0;
    while (col < MaxCol && snapshot[col][row] != Color())
      ++col;
    Color c;
    while (col < MaxCol) {
      if (c != snapshot[col][row]) {
        c = snapshot[col][row];
        n = 1;
      } else if (c != Color()) {
        ++n;
        const int KillLimit = 4;
        if (n >= KillLimit) {
          for (int i = 0; i < n; ++i)
            erasures.emplace_back(row, col - i);
        }
      }
      ++col;
    }
  }

  for (const auto &e : erasures) {
    auto p = pillAt(e);
    assert(p);
    p->erase(e);
  }

  pills_.erase(remove_if(begin(pills_), end(pills_), [&](const Pill &p) {
                 return p.leftColor() == Color() && p.rightColor() == Color();
               }),
               end(pills_));

  return !erasures.empty();
}

bool DrMarioApp::canDrop(const Pill &pill) const {
  Vec2i next{ pill.x(), pill.y() + 1 };
  if (next.y < MaxCol) {
    for (const auto &p : pills_) {
      if (&p != &pill && p.coord() == next)
        return false;
    }
    return true;
  }
  return false;
}

Pill *DrMarioApp::pillAt(Vec2f loc) {
  for (auto &p : pills_) {
    if (p.leftLocation() == loc || p.rightLocation() == loc)
      return &p;
  }
  return nullptr;
}


Pill *DrMarioApp::pillAt(float x, float y) {
  Vec2f loc{ x, y };
  return pillAt(loc);
}

const Color &DrMarioApp::colorAt(float x, float y) {
  Vec2f loc{ x, y };
  for (const auto &p : pills_) {
    if (p.leftLocation() == loc)
      return p.leftColor();
    else if (p.rightLocation() == loc)
      return p.rightColor();
  }
  return Black;
}

void DrMarioApp::eraseVertical(float x, float y, tuple<int, int> range) {
  for (int i = ::get<0>(range); i < ::get<1>(range); ++i) {
    Vec2f loc{ x, y + i *Pill::Height };
    auto pill = pillAt(loc);
    assert(pill);
    pill->erase(loc);
  }
}

void DrMarioApp::eraseHorizontal(float x, float y, tuple<int, int> range) {
  for (int i = ::get<0>(range); i < ::get<1>(range); ++i) {
    Vec2f loc{ x + i *Pill::Width, y };
    auto pill = pillAt(loc);
    assert(pill);
    pill->erase(loc);
  }
}

CINDER_APP_BASIC(DrMarioApp, RendererGl)
