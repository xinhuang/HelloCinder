#pragma once

#include "Control.h"

#include <cinder/Rect.h>
#include <cinder/Color.h>
#include <cinder/Vector.h>
#include <cinder/Font.h>

#include <memory>
#include <string>

class Label : public Control {
  struct Data;
  std::unique_ptr<Data> d;

  void updateCache() const;

public:
  Label();
  ~Label();

  void draw() const;

  void setText(const std::string &text);
  void setForeColor(const ci::Color &color);
  void setBackColor(const ci::Color &color);
  void setSize(const ci::Vec2f &size);

  const ci::Font &font() const;
  void setFont(const ci::Font &font);

  ci::Rectf rect() const {
    return { location(), location() + size() };
  }
};