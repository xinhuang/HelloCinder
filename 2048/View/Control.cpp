#include "Control.h"

#include <vector>

using namespace std;

using namespace ci;

struct Control::Data {
  Anchor anchor = Anchor::NONE;
  Vec2f location;
  Vec2f size;
  vector<ControlRef> children;
  Control *parent;
};

Control::Control() : d(make_unique<Data>()) {}

Control::~Control() {}

void Control::add(ControlRef child) {
  d->children.push_back(move(child));
  d->children.back()->setParent(this);
}

Control *Control::parent() { return d->parent; }
void Control::setParent(Control *parent) { d->parent = move(parent); }

const ci::Vec2f &Control::location() const { return d->location; }
void Control::setLocation(const ci::Vec2f &loc) { d->location = loc; }

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

Anchor Control::anchor() const { return d->anchor; }
void Control::setAnchor(Anchor anchor) { d->anchor = anchor; }

// TODO: should draw be const?
void Control::draw() {
  for (auto &child : d->children) {
    child->draw();
  }
}
