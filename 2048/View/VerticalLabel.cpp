#include "VerticalLabel.h"

using namespace ci;

VerticalLabel::VerticalLabel() {
  label = create<Label>();
  text = create<Label>();

  label->setSize({ size().x, size().y / 2 });
  performLayout();
}

VerticalLabel::~VerticalLabel() {}

void VerticalLabel::performLayout() {
  label->setLocation({ 0.f, 0.f });
  label->setAnchor(Anchor::LEFT_RIGHT | Anchor::TOP);

  text->setSize({ size().x, size().y - label->size().y });
  text->setLocation({ 0.f, label->size().y });
  text->setAnchor(Anchor::LEFT_RIGHT | Anchor::TOP);
}

void VerticalLabel::setLabelHeight(float height) {
  label->setSize({ label->size().x, height });
  performLayout();
}

Vec2f VerticalLabel::textSize() const { return text->size(); }

Vec2f VerticalLabel::textLocation() const { return text->location(); }

void VerticalLabel::setSize(const ci::Vec2f &size) {
  Control::setSize(size);
  label->setSize({ size.x, label->size().y });
  text->setSize({ size.x, text->size().y });
  performLayout();
}

void VerticalLabel::setLabelColor(const ci::Color &color) {
  label->setForeColor(color);
}

void VerticalLabel::setTextColor(const ci::Color &color) {
  text->setForeColor(color);
}

void VerticalLabel::setBackColor(const ci::Color &color) {
  label->setBackColor(color);
  text->setBackColor(color);
}

void VerticalLabel::setLabelFont(const ci::Font &font) { label->setFont(font); }

void VerticalLabel::setTextFont(const ci::Font &font) { text->setFont(font); }

void VerticalLabel::setLabel(const std::string &text) { label->setText(text); }

void VerticalLabel::setText(const std::string &text) {
  this->text->setText(text);
}
