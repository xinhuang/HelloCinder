#pragma once

#include "../View/IRenderable.h"
#include "../View/Animation.h"

#include <memory>

class Piece;

class Cell {
  const ci::Vec2i coord_;
  std::unique_ptr<Piece> piece_;
  Animation animation_;

public:
  Cell(const ci::Vec2i &position);

  const std::unique_ptr<Piece> &piece() const;
  const ci::Vec2i &coord() const;

  void draw(const ci::Rectf &rect);
  void place(std::unique_ptr<Piece> &&p);
  void moveTo(Cell &cell);
  void mergeTo(Cell &cell);

  int value() const;
};
