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

class Control {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Control();
  virtual ~Control();

  ci::Vec2f location() const;
  void setLocation(const ci::Vec2f &loc);

  ci::Vec2f size() const;
  void setSize(const ci::Vec2f &size);

  Anchor anchor() const;
  void setAnchor(Anchor anchor);
};

using ControlRef = std::shared_ptr<Control>;
