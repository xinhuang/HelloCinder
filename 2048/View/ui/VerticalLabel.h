#pragma once

#include "Control.h"
#include "Label.h"
#include "IntLabel.h"

#include <string>

#include <cinder/Vector.h>
#include <cinder/Color.h>
#include <cinder/Font.h>

namespace ui {

class VerticalLabel : public Control {
  std::shared_ptr<Label> name;
  std::shared_ptr<IntLabel> value;

  void performLayout();

public:
  VerticalLabel();
  ~VerticalLabel();

  void setLabelHeight(float height);
  ci::Vec2f textSize() const;
  ci::Vec2f textLocation() const;
  void setSize(const ci::Vec2f &size);
  void setLabelColor(const ci::Color &color);
  void setTextColor(const ci::Color &color);
  void setBackColor(const ci::ColorA &color);
  void setLabel(const std::string &text);
  void setLabelFont(const ci::Font &font);
  void setTextFont(const ci::Font &font);
  void setValue(int value);
};

}
