#pragma once

#include <cinder/cinder.h>
#include <cinder/app/AppBasic.h>

using namespace ci;
using namespace ci::app;

#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

class Pill {
  Vec2i coord_;
  Vec2f location_;
  Color lhs_, rhs_;

public:
  static const float Height;
  static const float Width;

  Pill(Vec2i coord, Color left, Color right)
      : coord_(move(coord)), location_(coord_.x * Width, coord_.y * Height),
        lhs_(left), rhs_(right) {}

  const Vec2i &coord() const { return coord_; }
  int x() const { return coord_.x; }
  int y() const { return coord_.y; }

  void draw();

  void erase(Vec2f loc);
  void fill(vector<vector<Color> > &matrix) const {
    assert(coord_.y < (int)matrix.size());
    assert(coord_.x < (int)matrix[coord_.y].size());
    matrix[coord_.y][coord_.x] = leftColor();
    matrix[coord_.y][coord_.x + 1] = rightColor();
  }
  void drop() {
    location_.y += Height;
    coord_.y++;
  }

  const Vec2f &location() const { return location_; }
  void setLocation(const Vec2f &value) { location_ = value; }

  float height() const { return Height; }

  const Color &leftColor() const { return lhs_; }
  Vec2i leftLocation() const { return coord_; }
  const Color &rightColor() const { return rhs_; }
  Vec2i rightLocation() const {
    return { coord_.x + 1, coord_.y };
  }

private:
  Rectf leftRect() const;
  Rectf rightRect() const;
};
