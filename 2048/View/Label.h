#pragma once

#include <cinder/Rect.h>
#include <cinder/Color.h>

#include <memory>
#include <string>

class Label {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Label();
  ~Label();

  void draw(const ci::Rectf& rect) const;

  void setText(const std::string& text);
  void setForeColor(const ci::Color& color);
  void setBackColor(const ci::Color& color);
  void setFontSize(float size);
};