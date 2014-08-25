#include "VerticalLabel.h"

using namespace ci;

namespace ui {

VerticalLabel::VerticalLabel() {
  name = create<Label>();
  value = create<IntLabel>();

  name->setSize({ size().x, size().y / 2 });
  performLayout();
}

VerticalLabel::~VerticalLabel() {}

void VerticalLabel::performLayout() {
  name->setLocation({ 0.f, 0.f });
  name->setAnchor(Anchor::LEFT_RIGHT | Anchor::TOP);

  value->setSize({ size().x, size().y - name->size().y });
  value->setLocation({ 0.f, name->size().y });
  value->setAnchor(Anchor::LEFT_RIGHT | Anchor::TOP);
}

void VerticalLabel::setLabelHeight(float height) {
  name->setSize({ name->size().x, height });
  performLayout();
}

Vec2f VerticalLabel::textSize() const { return value->size(); }

Vec2f VerticalLabel::textLocation() const { return value->location(); }

void VerticalLabel::setSize(const ci::Vec2f &size) {
  Control::setSize(size);
  name->setSize({ size.x, name->size().y });
  value->setSize({ size.x, value->size().y });
  performLayout();
}

void VerticalLabel::setLabelColor(const ci::Color &color) {
  name->setForeColor(color);
}

void VerticalLabel::setTextColor(const ci::Color &color) {
  value->setForeColor(color);
}

void VerticalLabel::setBackColor(const ci::ColorA &color) {
  name->setBackColor(color);
  value->setBackColor(color);
}

void VerticalLabel::setLabelFont(const ci::Font &font) { name->setFont(font); }

void VerticalLabel::setTextFont(const ci::Font &font) { value->setFont(font); }

void VerticalLabel::setLabel(const std::string &text) { name->setText(text); }

void VerticalLabel::setValue(int value) {
  this->value->setValue(value);
}

}
