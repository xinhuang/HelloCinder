#include "Cell.h"

#include "Piece.h"

#include "../View/CellAnimation.h"
#include "BoardLayout.h"

#include <cinder/app/app.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;

using namespace std;

Cell::Cell(const ci::Vec2i &coord) : coord_(coord) {
  sprite_ = createEmptyCellSprite();
  setRect(BoardLayout::cellRect(coord_));
}

const std::unique_ptr<Piece> &Cell::piece() const { return piece_; }

const ci::Vec2i &Cell::coord() const { return coord_; }

void Cell::setRect(const ci::Rectf &rect) {
  sprite_.setRect(rect);
}

void Cell::place(std::unique_ptr<Piece> &&p) {
  assert(!piece_);
  piece_ = move(p);
  piece_sprite_ = createPlacePieceSprite(value());
  piece_sprite_.setDepth(1);
  piece_sprite_.setRect(sprite_.rect());
}

void Cell::moveTo(Cell &cell) {
  assert(piece_);
  cell.piece_ = std::move(piece_);
  cell.piece_sprite_ = createMovePieceSprite(*this, cell);
  cell.piece_sprite_.setDepth(1);
  cell.piece_sprite_.setRect(cell.sprite_.rect());
  piece_sprite_ = {};
}

void Cell::mergeTo(Cell &cell) {
  assert(cell.piece_);
  assert(!cell.piece_->merged);
  assert(piece_);
  cell.piece_->merged = std::move(piece_);
  cell.piece_sprite_ = createMergeSprite(cell.piece_sprite_, *this, cell);
  cell.piece_sprite_.setDepth(1);
  cell.piece_sprite_.setRect(cell.sprite_.rect());
  piece_sprite_ = {};
}

int Cell::value() const {
  assert(piece_);
  return piece_->value;
}
