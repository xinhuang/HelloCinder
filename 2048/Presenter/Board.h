#pragma once

#include <memory>

#include <cinder/Rect.h>
#include <cinder/Vector.h>

class Cell;
class Piece;

class Board {
  struct Data;
  std::unique_ptr<Data> d;

  Cell& at(const ci::Vec2i& coord);
  const Cell &at(const ci::Vec2i &coord) const;

public:
  Board();
  Board(int width, int height);
  ~Board();

  Board& operator=(Board&& board);

  int width() const;
  int height() const;

  void place(const ci::Vec2i& coord, std::unique_ptr<Piece>&& p);
  bool moveAll(const ci::Vec2i &dir);
  bool moveToFurthest(ci::Vec2i src, const ci::Vec2i &dir);
  std::vector<int> buildTraversals(int max, int dir) const;
  void spawn();
  std::vector<ci::Vec2i> getFreeSpaces() const;
  bool isOccupied(const ci::Vec2i& coord) const;

  void draw(const ci::Rectf& rect);
};
