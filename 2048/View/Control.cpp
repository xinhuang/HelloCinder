#include "Control.h"

using namespace std;

using namespace ci;

struct Control::Data {
  Anchor anchor = Anchor::NONE;
  Vec2f location;
  Vec2f size;
};

Control::Control() : d(make_unique<Data>()) {}

Control::~Control() {}

const ci::Vec2f &Control::location() const { return d->location; }
void Control::setLocation(const ci::Vec2f &loc) { d->location = loc; }

const ci::Vec2f &Control::size() const { return d->size; }
void Control::setSize(const ci::Vec2f &size) {
  if ((int)(d->anchor) & (int)(Anchor::RIGHT)) {
    d->location.x -= size.x - d->size.x;
  }
  if ((int)(d->anchor) & (int)(Anchor::BOTTOM)) {
    d->location.y -= size.y - d->size.y;
  }
  d->size = size;
}

Anchor Control::anchor() const { return d->anchor; }
void Control::setAnchor(Anchor anchor) { d->anchor = anchor; }
