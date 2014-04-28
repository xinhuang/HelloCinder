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
  animation_ = emptyCellAnimation();
}

const std::unique_ptr<Piece> &Cell::piece() const { return piece_; }

const ci::Vec2i &Cell::coord() const { return coord_; }

void Cell::draw(const ci::Rectf &rect) {
  gl::enableAlphaBlending();
  gl::color(Color::white());
  gl::setViewport(getWindowBounds());
  animation_.draw(rect);
  gl::disableAlphaBlending();
}

void Cell::place(std::unique_ptr<Piece> &&p) {
  assert(!piece_);
  piece_ = move(p);
  animation_ += placePieceAnimation(value());
}

void Cell::moveTo(Cell &cell) {
  assert(piece_);
  cell.piece_ = std::move(piece_);
  cell.animation_ += movePieceAnimation(*this, cell);
  animation_ = emptyCellAnimation();
}

void Cell::mergeTo(Cell &cell) {
  assert(cell.piece_);
  assert(!cell.piece_->merged);
  assert(piece_);
  cell.piece_->merged = std::move(piece_);
  cell.animation_ = mergeAnimation(*this, cell) * cell.animation_;
  animation_ = emptyCellAnimation();
}

int Cell::value() const {
  assert(piece_);
  return piece_->value;
}
