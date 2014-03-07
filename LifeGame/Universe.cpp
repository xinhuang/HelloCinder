#include "stdafx.h"
#include "Universe.h"

#include <limits>
#include <future>
#include <cassert>

using namespace std;

#include <cinder/Surface.h>
#include <cinder/Channel.h>

using namespace ci;

struct Universe::Data {
  ~Data() {}

  Channel channel_;
  Rule rule_;
};

Universe::Universe() : d(make_unique<Data>()) {}

Universe::Universe(int width, int height) : Universe() {
  d->channel_ = Channel(width, height);

  auto dest = d->channel_.getData();
  const auto &bounds = d->channel_.getBounds();
  fill(dest, dest + d->channel_.getRowBytes() * bounds.getHeight(), 0x00);
}

Universe::Universe(Universe &&u) : d(move(u.d)) {}

Universe &Universe::operator=(Universe &&u) {
  swap(d, u.d);
  return *this;
}

Universe::Universe(const Universe &u) : d(make_unique<Data>()) {
  d->channel_ = u.d->channel_.clone();
}

Universe &Universe::operator=(Universe &u) {
  d->channel_ = u.d->channel_.clone();
  return *this;
}

Universe::~Universe() {}

int Universe::size() const { return 0; }

gl::Texture Universe::texture() { return gl::Texture(d->channel_); }

void Universe::next(Universe &u) {
  if (!(d->channel_.getBounds() == u.d->channel_.getBounds()))
    u = Universe(*this);

  const auto &bounds = d->channel_.getBounds();

  auto src = d->channel_.getData();
  auto cbSrcRow = d->channel_.getRowBytes();
  auto cbSrcInc = d->channel_.getIncrement();

  auto dest = u.d->channel_.getData();
  auto cbDestRow = u.d->channel_.getRowBytes();
  auto cbDestInc = u.d->channel_.getIncrement();

  fill(dest, dest + cbDestRow * bounds.getHeight(), 0x00);

  for (int r = 1; r < bounds.getHeight() - 1; ++r) {
    for (int c = 1; c < bounds.getWidth() - 1; ++c) {
      auto pixel = dest + r * cbDestRow + cbDestInc * c;
      auto srcpixel = src + r * cbDestRow + cbDestInc * c;
      if (d->rule_.nextGeneration(
              *srcpixel, *(srcpixel - cbSrcRow - cbSrcInc),
              *(srcpixel - cbSrcRow), *(srcpixel - cbSrcRow + cbSrcInc),
              *(srcpixel - cbSrcInc), *(srcpixel + cbSrcInc),
              *(srcpixel + cbSrcRow - cbSrcInc), *(srcpixel + cbSrcRow),
              *(srcpixel + cbSrcRow + cbSrcInc))) {
        *pixel = 0xFF;
      } else {
        *pixel = 0x00;
      }
    }
  }
}

int Universe::width() const { return d->channel_.getWidth(); }

int Universe::height() const { return d->channel_.getHeight(); }

void Universe::add(const Point &p) {
  assert(p.x < width() && p.x >= 0);
  assert(p.y < height() && p.y >= 0);
  d->channel_.setValue({ p.x, p.y }, 0xFF);
}

ostream &operator<<(ostream &os, const Universe &u) { return os; }
