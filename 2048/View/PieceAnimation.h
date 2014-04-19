#pragma once

#include "../Presenter/Environment.h"
#include "Animation.h"

class PlacePieceAnimation : public IRenderable {
  Animation animation_;

public:
  PlacePieceAnimation(const Piece &p) {
    auto tex = PieceRenderer::instance().render(
        p.value, Environment::instance().cellSize());
    animation_ = Animation().zoom(tex, 0.5f, 1.03f, 3).zoom(tex, 1.03f, 1.f, 2);
  }
  void draw(const ci::Rectf &rect) final {
    animation_.draw(rect);
  }
};

class MovePieceAnimation : public IRenderable {
  int value;

public:
  MovePieceAnimation(const Piece &p) { value = p.value; }
  void draw(const ci::Rectf &rect) final {
    PieceRenderer::instance().draw(value, rect);
  }
};

class MergeAnimation : public IRenderable {
  int value;

public:
  MergeAnimation(const Piece &p) { value = p.value * 2; }
  void draw(const ci::Rectf &rect) final {
    PieceRenderer::instance().draw(value, rect);
  }
};

class EmptyCellAnimation : public IRenderable {
public:
  void draw(const ci::Rectf &rect) final {}
};