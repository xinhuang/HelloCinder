#include "stdafx.h"
#include "Universe.h"

#include <limits>
#include <future>
#include <cassert>

using namespace std;

#include <cinder/Surface.h>
#include <cinder/Channel.h>

using namespace ci;

#define _IPP_SEQUENTIAL_STATIC
#include <ipp.h>

#include "Random.h"
#include "GameConfig.h"

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

int Universe::size() const { return width() * height(); }

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

  auto height = bounds.getHeight();
  auto width = bounds.getWidth();

  for (int r = 0; r < height; ++r) {
    *(dest + r *cbDestRow) = 0x00;
    *(dest + r *cbDestRow + (width - 1) *cbDestInc) = 0x00;
  }
  fill(dest, dest + (width - 1) * cbDestInc, 0x00);
  fill(dest + (height - 1) * cbDestRow,
       dest + (height - 1) * cbDestRow + (width - 1) * cbDestInc, 0x00);

//  auto &rule = d->rule_;
//#pragma omp parallel for
//  for (int r = 1; r < height - 1; ++r) {
//    for (int c = 1; c < width - 1; ++c) {
//      auto pixel = dest + r * cbDestRow + cbDestInc * c;
//      auto srcpixel = src + r * cbDestRow + cbDestInc * c;
//      if (rule.nextGeneration(
//              *srcpixel != 0, *(srcpixel - cbSrcRow - cbSrcInc) != 0,
//              *(srcpixel - cbSrcRow) != 0,
//              *(srcpixel - cbSrcRow + cbSrcInc) != 0,
//              *(srcpixel - cbSrcInc) != 0, *(srcpixel + cbSrcInc) != 0,
//              *(srcpixel + cbSrcRow - cbSrcInc) != 0,
//              *(srcpixel + cbSrcRow) != 0,
//              *(srcpixel + cbSrcRow + cbSrcInc) != 0)) {
//        *pixel = 0xFF;
//      } else {
//        *pixel = 0x00;
//      }
//    }
//  }

  const auto stride = cbSrcRow;
  IppiSize roi = { stride - 2, height - 2 };

  ippiAndC_8u_C1R(src, stride, 1, src, stride, { width, height });      // map 255 -> 1

  auto temp = ippsMalloc_8u(width * height);

  // fill dest with zero
  ippiSet_8u_C1R(0, temp, stride, { width, height });
  ippiSet_8u_C1R(0, dest, stride, roi);

  int offset = stride + 1 * cbSrcInc;

  int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
  int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };
  for (int i = 0; i < 8; ++i) {
    // add src to dest with offset [x, y]
    auto ptr = src + offset + x[i] * cbSrcInc + y[i] * stride;
    ippiAdd_8u_C1RSfs(ptr, stride, dest + offset, stride, dest + offset, stride, roi, 0);
  }

  ippiCopy_8u_C1R(dest, stride, temp, stride, roi);

  // map 2 -> 1
  ippiCompareC_8u_C1R(temp, stride, 2, temp, stride, { width, height }, ippCmpEq);
  ippiAnd_8u_C1R(src, stride, temp, stride, temp, stride, { width, height });

  // map 3 -> 1
  ippiCompareC_8u_C1R(dest, stride, 3, dest, stride, { width, height }, ippCmpEq);

  // combine 3s and 2s
  ippiOr_8u_C1R(temp, stride, dest, stride, dest, stride, { width, height });

  // map 1 to 255
  //ippiRGBToGray_8u_C3C1R(dest, stride, dest, stride, { width, height });

  ippsFree(temp);
}

int Universe::width() const { return d->channel_.getWidth(); }

int Universe::height() const { return d->channel_.getHeight(); }

void Universe::add(const Point &p) {
  assert(p.x < width() && p.x >= 0);
  assert(p.y < height() && p.y >= 0);
  d->channel_.setValue({ p.x, p.y }, 0xFF);
}

Universe Universe::bigBang(int width, int height) {
  Universe u(width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = Random::next<int>(width - 1);
    int y = Random::next<int>(height - 1);
    u.addn({ x, y });
  }
  return u;
}

ostream &operator<<(ostream &os, const Universe &u) { return os; }
