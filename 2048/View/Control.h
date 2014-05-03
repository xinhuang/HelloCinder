#pragma once

#include <cinder/vector.h>

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

class Control;
using ControlRef = std::shared_ptr<Control>;

class Control : public std::enable_shared_from_this<Control> {
  struct Data;
  std::unique_ptr<Data> d;

  void add(ControlRef child);

public:
  Control();
  virtual ~Control();

  const ci::Vec2f &location() const;
  void setLocation(const ci::Vec2f &loc);

  const ci::Vec2f &size() const;
  void setSize(const ci::Vec2f &size);

  Anchor anchor() const;
  void setAnchor(Anchor anchor);

  ControlRef shared() { return shared_from_this(); }
  const std::weak_ptr<Control> &parent();
  void setParent(std::weak_ptr<Control> parent);

  template <typename T>
  std::shared_ptr<T> create() {
    auto v = std::make_shared<T>();
    add(std::dynamic_pointer_cast<Control>(v));
    return v;
  }
};
