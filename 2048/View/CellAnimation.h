#pragma once

#include "Animation.h"

#include "../Presenter/Cell.h"

#include "../Presenter/Environment.h"

class Cell;

class PlaceCellAnimation : public IRenderable {
  Animation animation_;

public:
  PlaceCellAnimation(const Cell &c) {
    auto tex = PieceRenderer::instance().render(
        c.value(), Environment::instance().cellSize());
    animation_ = Animation().zoom(tex, 0.5f, 1.03f, 3).zoom(tex, 1.03f, 1.f, 2);
  }
  void draw(const ci::Rectf &rect) final { animation_.draw(rect); }
};

class MoveCellAnimation : public IRenderable {
  int value;

public:
  MoveCellAnimation(const Cell &from, const Cell &to) { value = from.value(); }
  void draw(const ci::Rectf &rect) final {
    PieceRenderer::instance().draw(value, rect);
  }
};

class MergeAnimation : public IRenderable {
  int value;

public:
  MergeAnimation(const Cell &c) { value = c.value() * 2; }
  void draw(const ci::Rectf &rect) final {
    PieceRenderer::instance().draw(value, rect);
  }
};

class EmptyCellAnimation : public IRenderable {
public:
  void draw(const ci::Rectf &rect) final {}
};