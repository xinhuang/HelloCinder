#include "Cell.h"

#include "../View/CellAnimation.h"
#include "../Model/Piece.h"

using namespace std;

Cell::Cell(const ci::Vec2i &position) : pos_(position) {
  animation_ = std::make_unique<EmptyCellAnimation>();
}

const std::unique_ptr<Piece> &Cell::piece() const { return piece_; }

const ci::Vec2i &Cell::pos() const { return pos_; }

void Cell::draw(const ci::Rectf &rect) { animation_->draw(rect); }

void Cell::place(std::unique_ptr<Piece> &&p) {
  assert(!piece_);
  piece_ = std::move(p);
  animation_ = std::make_unique<PlaceCellAnimation>(*this);
}

void Cell::moveTo(Cell &cell) {
  assert(piece_);
  cell.piece_ = std::move(piece_);
  cell.animation_ = std::make_unique<MoveCellAnimation>(*this, cell);
  animation_ = std::make_unique<EmptyCellAnimation>();
}

void Cell::mergeTo(Cell &cell) {
  assert(cell.piece_);
  assert(!cell.piece_->merged);
  assert(piece_);
  cell.piece_->merged = std::move(piece_);
  cell.animation_ = std::make_unique<MergeAnimation>(cell);
  animation_ = std::make_unique<EmptyCellAnimation>();
}

int Cell::value() const {
  assert(piece_);
  return piece_->value;
}
