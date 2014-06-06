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

  void draw() final;

  void setText(const std::string &text);
  void setForeColor(const ci::ColorA &color);
  void setBackColor(const ci::ColorA &color);
  const ci::ColorA& backColor() const;
  void setSize(const ci::Vec2f &size);
  void setCornerRadius(float value);

  const ci::Font &font() const;
  void setFont(const ci::Font &font);
};