#include "stdafx.h"
#include "Universe.h"

#include <limits>
#include <future>
#include <cassert>

using namespace std;

#include <cinder/Surface.h>
#include <cinder/Channel.h>

using namespace ci;

#include "vax.h"

//#include "tbb/task_scheduler_init.h"
//#include "tbb/blocked_range.h"
//#include "tbb/parallel_for.h"
//
//using namespace tbb;

#include "Random.h"
#include "GameConfig.h"

struct Universe::Data {
  ~Data() {}

  Channel channel_;
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
    *(src + r *cbSrcRow) = 0x00;
    *(src + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(src, src + (width - 1) * cbSrcInc, 0x00);
  fill(src + (height - 1) * cbSrcRow,
       src + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0xFF);

//  static const int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
//  static const int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };
//#pragma omp parallel for
//  for (int r = 1; r < height - 1; ++r) {
//    for (int c = 1; c < width - 1; ++c) {
//      auto pixel = dest + r * cbDestRow + cbDestInc * c;
//      auto srcpixel = src + r * cbSrcRow + cbSrcInc * c;
//      *pixel = 0;
//      for (int i = 0; i < 8; ++i) {
//        auto ptr = srcpixel + x[i] * cbSrcInc + y[i] * cbSrcRow;
//        if (*ptr > 0)
//          ++*pixel;
//      }
//      if (*pixel == 3)
//        *pixel = 0xFF;
//      else if (*pixel == 2)
//        *pixel = *srcpixel;
//      else
//        *pixel = 0x00;
//    }
//  }

  ippiAndC_8u_C1R(src, cbSrcRow, 1, src, cbSrcRow, { width, height });  // map 0xFF -> 1
  ippiSet_8u_C1R(0, dest, cbDestRow, { width, height });

  const int step = 1;
#pragma omp parallel for
  for (int r = 1; r < height - 1; r += step) {
      auto srcblock = src + r * cbSrcRow + cbSrcInc;
      auto destBlock = dest + r * cbDestRow + cbDestInc;
      next(srcblock, cbSrcRow, destBlock, cbDestRow, { width - 2, step });
  }

  //auto block = blocked_range<int>(1, height - 1);
  //parallel_for(block, [&](const blocked_range<int> &range) {
  //  auto srcblock = src + range.begin() * cbSrcRow + cbSrcInc;
  //  auto destBlock = dest + range.begin() * cbDestRow + cbDestInc;
  //  next(srcblock, cbSrcRow, destBlock, cbDestRow, { width - 2, range.end() - range.begin() });
  //});

  //auto srcblock = src + cbSrcRow + cbSrcInc;
  //auto destblock = dest + cbDestRow + cbDestInc;
  //next(srcblock, cbSrcRow, destblock, cbDestRow, { width - 2, height - 2 });
}

void Universe::next(uint8_t* src, int srcStride, uint8_t* dest, int destStride, const ci::Vec2i& _roi) const {
  IppiSize roi = { _roi.x, _roi.y };

  const int width = roi.width + 2;
  const int height = roi.height + 2;
  auto temp = ippsMalloc_8u(width * height);

  int offset = 1;

  int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
  int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };
  for (int i = 0; i < 8; ++i) {
    // add src to dest with offset [x, y]
    auto ptr = src + offset + x[i] * 1 + y[i] * srcStride;
    ippiAdd_8u_C1RSfs(ptr, srcStride, dest + offset, destStride, dest + offset, destStride, roi, 0);
  }

  ippiCopy_8u_C1R(dest, destStride, temp, srcStride, roi);

  // map 2 -> 255
  ippiCompareC_8u_C1R(temp, srcStride, 2, temp, srcStride, roi, ippCmpEq);
  ippiAnd_8u_C1R(src, srcStride, temp, srcStride, temp, srcStride, roi);

  // map 3 -> 255
  ippiCompareC_8u_C1R(dest, destStride, 3, dest, destStride, roi, ippCmpEq);

  // combine 3s and 2s
  ippiOr_8u_C1R(temp, srcStride, dest, destStride, dest, destStride, roi);

  ippsFree(temp);
}

int Universe::width() const { return d->channel_.getWidth(); }

int Universe::height() const { return d->channel_.getHeight(); }

void Universe::add(const Vec2i &p) {
  assert(p.x < width() && p.x >= 0);
  assert(p.y < height() && p.y >= 0);
  d->channel_.setValue({ p.x, p.y }, 0xFF);
}

Universe Universe::bigBang(int width, int height) {
  Universe u(width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = Random::next<int>(width - 1);
    int y = Random::next<int>(height - 1);
    u.add({ x, y });
  }
  return u;
}

ostream &operator<<(ostream &os, const Universe &u) { return os; }
