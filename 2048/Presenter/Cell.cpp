#include "Cell.h"

#include "Piece.h"

#include "../View/CellAnimation.h"

#include <cinder/app/app.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;

using namespace std;

Cell::Cell(const ci::Vec2i &coord) : coord_(coord) {
  anim_ = emptyCellAnimation2();
}

const std::unique_ptr<Piece> &Cell::piece() const { return piece_; }

const ci::Vec2i &Cell::coord() const { return coord_; }

void Cell::draw(const ci::Rectf &rect) {
  piece_anim_.draw(rect);
}

void Cell::drawBackground(const ci::Rectf &rect) {
  anim_.draw(rect);
}

void Cell::place(std::unique_ptr<Piece> &&p) {
  assert(!piece_);
  piece_ = move(p);
  piece_anim_ = placePieceAnimation2(value());
}

void Cell::moveTo(Cell &cell) {
  assert(piece_);
  cell.piece_ = std::move(piece_);
  cell.piece_anim_ = movePieceAnimation2(*this, cell);
  piece_anim_ = {};
}

void Cell::mergeTo(Cell &cell) {
  assert(cell.piece_);
  assert(!cell.piece_->merged);
  assert(piece_);
  cell.piece_->merged = std::move(piece_);
  // TODO: the first piece animation is ereased.
  // first: the one prior in the movement direction
  cell.piece_anim_ = mergeAnimation2(*this, cell);
  piece_anim_ = {};
}

int Cell::value() const {
  assert(piece_);
  return piece_->value;
}
