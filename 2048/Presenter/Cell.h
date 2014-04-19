#pragma once

#include "../Model/Piece.h"
#include "../View/PieceAnimation.h"

class Cell {
  const ci::Vec2i pos_;
  std::unique_ptr<Piece> piece_;
  std::unique_ptr<IRenderable> animation_;

public:
  Cell(const ci::Vec2i &position) : pos_(position) {
    animation_ = std::make_unique<EmptyCellAnimation>();
  }

  const std::unique_ptr<Piece> &piece() const { return piece_; }
  const ci::Vec2i &pos() const { return pos_; }

  void draw(const ci::Rectf &rect) { animation_->draw(rect); }
  void place(std::unique_ptr<Piece> &&p) {
    piece_ = std::move(p);
    animation_ = std::make_unique<PlacePieceAnimation>(*piece_);
  }
  void moveTo(Cell &cell) {
    cell.piece_ = std::move(piece_);
    cell.animation_ = std::make_unique<MovePieceAnimation>(*cell.piece_);
    animation_ = std::make_unique<EmptyCellAnimation>();
  }
  void mergeTo(Cell &cell) {
    cell.piece_->merged = std::move(piece_);
    cell.animation_ = std::make_unique<MergeAnimation>(*cell.piece_);
    animation_ = std::make_unique<EmptyCellAnimation>();
  }
};
