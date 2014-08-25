#include "Control.h"

#include <vector>
#include <algorithm>

using namespace std;

using namespace ci;

namespace ui {

struct Control::Data {
  bool visible = true;
  Anchor anchor = Anchor::NONE;
  Vec2f location;
  Vec2f size;
  vector<ControlRef> children;
  Control *parent = nullptr;
};

Control::Control() : d(make_unique<Data>()) {}

Control::~Control() {}

void Control::add(ControlRef child) {
  d->children.push_back(move(child));
  d->children.back()->setParent(this);
}

bool Control::visible() const { return d->visible; }

Control *Control::parent() { return d->parent; }
void Control::setParent(Control *parent) { d->parent = move(parent); }

ci::Vec2f Control::location() const { return d->location; }
void Control::setLocation(const ci::Vec2f &loc) { d->location = loc; }

ci::Rectf Control::screen(const ci::Rectf &rect) const {
  return rect + screen(rect.getUpperLeft());
}

ci::Vec2f Control::screen(const ci::Vec2f &pt) const {
  auto screen_pt = pt;
  for (auto control = this; control->d->parent; control = control->d->parent) {
    screen_pt += control->location();
  }
  return screen_pt;
}

const ci::Vec2f &Control::size() const { return d->size; }

void Control::setSize(const ci::Vec2f &size) {
  if (d->anchor == Anchor::RIGHT) {
    d->location.x -= size.x - d->size.x;
  }
  if (d->anchor == Anchor::BOTTOM) {
    d->location.y -= size.y - d->size.y;
  }

  // layout children
  for (auto &child : d->children) {
    switch (child->anchor()) {
    case Anchor::RIGHT: {
      // d->location.x -= size.x - d->size.x;
      Vec2f loc = { child->location().x + size.x - this->size().x,
                    child->location().y };
      child->setLocation(loc);
      break;
    }

    case Anchor::LEFT_RIGHT: {
      Vec2f child_size = { size.x - (this->size().x - child->location().x -
                                     child->size().x),
                           child->size().y };
      child->setSize(child_size);
      break;
    }
    }
  }

  d->size = size;
}

void Control::setRect(const ci::Rectf &rect) {
  setSize(rect.getSize());
  setLocation(rect.getUpperLeft());
}

Anchor Control::anchor() const { return d->anchor; }
void Control::setAnchor(Anchor anchor) { d->anchor = anchor; }

void Control::hide() { d->visible = false; }
void Control::show() { d->visible = true; }

// TODO: should draw be const?
void Control::draw() {
  for (auto &child : d->children) {
    if (child->visible()) {
      child->draw();
    }
  }
}

void Control::update() {
  for (auto &child : d->children) {
    child->update();
  }
}

void Control::bringToFront() {
  auto parent = this->parent();
  assert(parent);
  auto controls = parent->d->children;
  auto iter = find_if(controls.begin(), controls.end(),
                      [&](const ControlRef &c) { return c.get() == this; });
  assert(iter != controls.end());
  swap(*iter, *(controls.rbegin()));
}

void Control::sendToBottom() {
  auto parent = this->parent();
  assert(parent);
  auto controls = parent->d->children;
  auto iter = find_if(controls.begin(), controls.end(),
                      [&](const ControlRef &c) { return c.get() == this; });
  assert(iter != controls.end());
  swap(*iter, *(controls.begin()));
}
}
