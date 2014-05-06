#include "VerticalLabel.h"

VerticalLabel::VerticalLabel() {
  label = create<Label>();
  text = create<Label>();
  performLayout();
}

VerticalLabel::~VerticalLabel() {}

void VerticalLabel::performLayout() {
  label->setLocation({ 0.f, 0.f });
  label->setSize({ size().x, size().y / 2 });
  label->setAnchor(Anchor::LEFT_RIGHT | Anchor::TOP);

  text->setLocation({ 0.f, size().y / 2 });
  text->setSize({ size().x, size().y / 2 });
  text->setAnchor(Anchor::LEFT_RIGHT | Anchor::TOP);
}

void VerticalLabel::setSize(const ci::Vec2f &size) {
  Control::setSize(size);
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
