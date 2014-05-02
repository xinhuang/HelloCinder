#pragma once

#include "Piece.h"

#include "../View/IRenderable.h"
#include "../View/Sprite.h"

#include <memory>

class Cell {
  const ci::Vec2i coord_;
  std::unique_ptr<Piece> piece_;
  Sprite anim_;
  Sprite piece_anim_;

public:
  Cell(const ci::Vec2i &coord);
  Cell(Cell &&cell)
      : coord_(cell.coord_), piece_(std::move(cell.piece_)),
        piece_anim_(cell.piece_anim_), anim_(cell.anim_) {}

  const std::unique_ptr<Piece> &piece() const;
  const ci::Vec2i &coord() const;

  void draw(const ci::Rectf &rect);
  void drawBackground(const ci::Rectf &rect);
  void place(std::unique_ptr<Piece> &&p);
  void moveTo(Cell &cell);
  void mergeTo(Cell &cell);

  int value() const;
};
