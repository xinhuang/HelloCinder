#include "VerticalLabel.h"

VerticalLabel::VerticalLabel() {}

VerticalLabel::~VerticalLabel() {}

void VerticalLabel::setLocation(const ci::Vec2f &loc) {
  Control::setLocation(loc);
  label.setLocation(loc);
  auto label_height = 0.f;
  if (label.font())
    label_height = label.font().getDescent() + label.font().getSize();
  text.setLocation({ loc.x, loc.y + label_height });
}

void VerticalLabel::setSize(const ci::Vec2f &size) {
  Control::setSize(size);
  label.setSize({ size.x, size.y / 2 });
  text.setSize({ size.x, size.y / 2 });
  const auto& loc = label.location();
  auto label_height = 0.f;
  if (label.font())
    label_height = label.font().getDescent() + label.font().getSize();
  text.setLocation({ loc.x, loc.y + label_height });
}

void VerticalLabel::setLabelColor(const ci::Color &color) {
  label.setForeColor(color);
}

void VerticalLabel::setTextColor(const ci::Color &color) {
  text.setForeColor(color);
}

void VerticalLabel::setBackColor(const ci::Color &color) {
  label.setBackColor(color);
  text.setBackColor(color);
}

void VerticalLabel::setLabelFont(const ci::Font& font) {
  label.setFont(font);
  setLocation(location());
}

void VerticalLabel::setTextFont(const ci::Font& font) {
  text.setFont(font);
  setLocation(location());
}

void VerticalLabel::setLabel(const std::string &text) {
  label.setText(text);
}

void VerticalLabel::setText(const std::string &text) {
  this->text.setText(text);
}

void VerticalLabel::draw() const {
  label.draw();
  text.draw();
}