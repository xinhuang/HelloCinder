#pragma once

#include <cinder/vector.h>
#include <cinder/Rect.h>

enum class Anchor {
  NONE = 0x00,
  LEFT = 0x01,
  RIGHT = 0x02,
  TOP = 0x04,
  BOTTOM = 0x08,
  LEFT_RIGHT = LEFT | RIGHT,
  TOP_BOTTOM = TOP | BOTTOM,
  ALL = LEFT_RIGHT | TOP_BOTTOM,
};

inline Anchor operator | (Anchor lhs, Anchor rhs) {
  return (Anchor)((int)lhs | (int)rhs);
}

inline bool operator & (Anchor lhs, Anchor rhs) {
  return ((int)lhs & (int)rhs) != 0;
}

class Control;
using ControlRef = std::shared_ptr<Control>;

class Control {
  struct Data;
  std::unique_ptr<Data> d;

  void add(ControlRef child);

protected:
  ci::Vec2f screen(const ci::Vec2f& pt) const;
  ci::Rectf screen(const ci::Rectf& rect) const;

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

  Anchor anchor() const;
  void setAnchor(Anchor anchor);

  Control* parent();
  void setParent(Control* parent);

  virtual void draw();

  template <typename T>
  std::shared_ptr<T> create() {
    auto v = std::make_shared<T>();
    add(std::dynamic_pointer_cast<Control>(v));
    return v;
  }
};
