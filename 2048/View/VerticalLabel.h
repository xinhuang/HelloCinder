#pragma once

#include "Control.h"
#include "Label.h"

#include <string>

#include <cinder/Vector.h>
#include <cinder/Color.h>
#include <cinder/Font.h>

class VerticalLabel : public Control {
  std::shared_ptr<Label> label, text;

  void performLayout();

public:
  VerticalLabel();
  ~VerticalLabel();

  void setLabelHeight(float height);
  ci::Vec2f textSize() const;
  void setSize(const ci::Vec2f &size);
  void setLabelColor(const ci::Color &color);
  void setTextColor(const ci::Color &color);
  void setBackColor(const ci::Color &color);
  void setLabel(const std::string &text);
  void setLabelFont(const ci::Font &font);
  void setTextFont(const ci::Font &font);
  void setText(const std::string &text);
};
