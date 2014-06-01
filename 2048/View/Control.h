#pragma once

#include <cinder/vector.h>
#include <cinder/Rect.h>

#include "Anchor.h"

class Control;
using ControlRef = std::shared_ptr<Control>;

class Control {
  struct Data;
  std::unique_ptr<Data> d;

  void add(ControlRef child);

protected:
  ci::Vec2f screen(const ci::Vec2f &pt) const;
  ci::Rectf screen(const ci::Rectf &rect) const;

public:
  Control();
  virtual ~Control();

  ci::Vec2f location() const;
  void setLocation(const ci::Vec2f &loc);

  const ci::Vec2f &size() const;
  void setSize(const ci::Vec2f &size);

  ci::Rectf rect() const {
    return { location(), location() + size() };
  }
  void setRect(const ci::Rectf& rect);

  Anchor anchor() const;
  void setAnchor(Anchor anchor);

  Control *parent();
  void setParent(Control *parent);

  void hide();
  void show();
  bool visible() const;

  virtual void draw();

  template <typename T, typename... Vs> std::shared_ptr<T> create(Vs &&... vs) {
    auto v = std::make_shared<T>(std::forward<Vs>(vs)...);
    add(std::dynamic_pointer_cast<Control>(v));
    return v;
  }
};
