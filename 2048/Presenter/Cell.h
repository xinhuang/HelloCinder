#pragma once

#include "Piece.h"

#include "../View/Sprite.h"

#include <memory>

class Cell {
  const ci::Vec2i coord_;
  std::unique_ptr<Piece> piece_;
  animation::Sprite sprite_;
  // TODO: lift piece_anim_ to Piece class
  animation::Sprite piece_sprite_;

public:
  Cell(const ci::Vec2i &coord);
  Cell(Cell &&cell)
      : coord_(cell.coord_), piece_(std::move(cell.piece_)),
        piece_sprite_(cell.piece_sprite_), sprite_(cell.sprite_) {}

  const std::unique_ptr<Piece> &piece() const;
  const ci::Vec2i &coord() const;

  void setRect(const ci::Rectf &rect);
  void place(std::unique_ptr<Piece> &&p);
  void moveTo(Cell &cell);
  void mergeTo(Cell &cell);

  int value() const;
};
