#pragma once

#include "../View/IRenderable.h"

#include <memory>

class Piece;

class Cell {
  const ci::Vec2i pos_;
  std::unique_ptr<Piece> piece_;
  std::unique_ptr<IRenderable> animation_;

public:
  Cell(const ci::Vec2i &position);

  const std::unique_ptr<Piece> &piece() const;
  const ci::Vec2i &pos() const;

  void draw(const ci::Rectf &rect);
  void place(std::unique_ptr<Piece> &&p);
  void moveTo(Cell &cell);
  void mergeTo(Cell &cell);

  int value() const;
};
