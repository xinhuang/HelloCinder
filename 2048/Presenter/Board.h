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
  void place(const ci::Vec2i& coord, std::unique_ptr<Piece>&& p);
  bool slide(ci::Vec2i src, const ci::Vec2i &dir);
  std::vector<int> traversal(int max, int dir) const;
  std::vector<ci::Vec2i> freespaces() const;
  bool isOccupied(const ci::Vec2i& coord) const;

public:
  Board();
  Board(int width, int height);
  ~Board();

  Board& operator=(Board&& board);

  int width() const;
  int height() const;
  bool moves_available() const;

  bool slide(const ci::Vec2i &dir);
  void spawn();

  void draw(const ci::Rectf& rect);
};
