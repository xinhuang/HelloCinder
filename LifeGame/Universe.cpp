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
// using namespace tbb;

#include "Random.h"
#include "GameConfig.h"

struct AbstractCpuUniverse::Data {
  ~Data() {}

  Channel channels_[2];

  Channel &src() {
    assert(current_ < sizeof(channels_) / sizeof(channels_[0]));
    return channels_[current_];
  }

  Channel &dst() {
    assert(current_ < sizeof(channels_) / sizeof(channels_[0]));
    return channels_[1 - current_];
  }

  void flip() { current_ = (current_ + 1) % 2; }

  void init(Channel &c, int width, int height) const {
    c = Channel(width, height);

    auto dest = c.getData();
    const auto &bounds = c.getBounds();
    fill(dest, dest + c.getRowBytes() * bounds.getHeight(), 0x00);
  }

private:
  int current_ = 0;
};

AbstractCpuUniverse::AbstractCpuUniverse() : d(make_unique<Data>()) {}

AbstractCpuUniverse::AbstractCpuUniverse(int width, int height)
    : AbstractCpuUniverse() {
  d->init(d->channels_[0], width, height);
  d->init(d->channels_[1], width, height);
}

AbstractCpuUniverse::AbstractCpuUniverse(AbstractCpuUniverse &&u)
    : d(move(u.d)) {}

AbstractCpuUniverse &AbstractCpuUniverse::operator=(AbstractCpuUniverse &&u) {
  swap(d, u.d);
  return *this;
}

AbstractCpuUniverse::~AbstractCpuUniverse() {}

int AbstractCpuUniverse::size() const { return width() * height(); }

gl::Texture AbstractCpuUniverse::texture() const {
  return gl::Texture(d->src());
}

void AbstractCpuUniverse::next() {
  next(d->src(), d->dst());
  d->flip();
}

void AbstractCpuUniverse::nextLoop(ci::Channel &src, ci::Channel &dst) const {
  assert(src.getBounds() == dst.getBounds());

  const auto &bounds = src.getBounds();

  auto pSrcData = src.getData();
  auto cbSrcRow = src.getRowBytes();
  auto cbSrcInc = src.getIncrement();

  auto pDstData = dst.getData();
  auto cbDestRow = dst.getRowBytes();
  auto cbDestInc = dst.getIncrement();

  auto height = bounds.getHeight();
  auto width = bounds.getWidth();

  for (int r = 0; r < height; ++r) {
    *(pSrcData + r *cbSrcRow) = 0x00;
    *(pSrcData + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
  fill(pSrcData + (height - 1) * cbSrcRow,
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0xFF);

  static const int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
  static const int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };
  for (int r = 1; r < height - 1; ++r) {
    for (int c = 1; c < width - 1; ++c) {
      auto pixel = pDstData + r * cbDestRow + cbDestInc * c;
      auto srcpixel = pSrcData + r * cbSrcRow + cbSrcInc * c;
      int count = 0;
      for (int i = 0; i < 8; ++i) {
        auto ptr = srcpixel + x[i] * cbSrcInc + y[i] * cbSrcRow;
        if (*ptr == 0xFF)
          ++count;
      }

      switch (count) {
      case 3:
        *pixel = 0xFF;
        break;

      case 2:
        *pixel = *srcpixel;
        break;

      default:
        *pixel = 0x00;
        break;
      }
    }
  }
}

void AbstractCpuUniverse::nextLoopOmp(ci::Channel &src,
                                      ci::Channel &dst) const {
  assert(src.getBounds() == dst.getBounds());

  const auto &bounds = src.getBounds();

  auto pSrcData = src.getData();
  auto cbSrcRow = src.getRowBytes();
  auto cbSrcInc = src.getIncrement();

  auto pDstData = dst.getData();
  auto cbDestRow = dst.getRowBytes();
  auto cbDestInc = dst.getIncrement();

  auto height = bounds.getHeight();
  auto width = bounds.getWidth();

  for (int r = 0; r < height; ++r) {
    *(pSrcData + r *cbSrcRow) = 0x00;
    *(pSrcData + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
  fill(pSrcData + (height - 1) * cbSrcRow,
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0xFF);

  static const int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
  static const int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };
#pragma omp parallel for
  for (int r = 1; r < height - 1; ++r) {
    for (int c = 1; c < width - 1; ++c) {
      auto pixel = pDstData + r * cbDestRow + cbDestInc * c;
      auto srcpixel = pSrcData + r * cbSrcRow + cbSrcInc * c;
      *pixel = 0;
      for (int i = 0; i < 8; ++i) {
        auto ptr = srcpixel + x[i] * cbSrcInc + y[i] * cbSrcRow;
        if (*ptr > 0)
          ++*pixel;
      }
      if (*pixel == 3)
        *pixel = 0xFF;
      else if (*pixel == 2)
        *pixel = *srcpixel;
      else
        *pixel = 0x00;
    }
  }
}

void AbstractCpuUniverse::nextIpp(ci::Channel &src, ci::Channel &dst) const {
  assert(src.getBounds() == dst.getBounds());

  const auto &bounds = src.getBounds();

  auto pSrcData = src.getData();
  auto cbSrcRow = src.getRowBytes();
  auto cbSrcInc = src.getIncrement();

  auto pDstData = dst.getData();
  auto cbDestRow = dst.getRowBytes();
  auto cbDestInc = dst.getIncrement();

  auto height = bounds.getHeight();
  auto width = bounds.getWidth();

  for (int r = 0; r < height; ++r) {
    *(pSrcData + r *cbSrcRow) = 0x00;
    *(pSrcData + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
  fill(pSrcData + (height - 1) * cbSrcRow,
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0xFF);

  ippiAndC_8u_C1R(pSrcData, cbSrcRow, 1, pSrcData, cbSrcRow,
                  { width, height }); // map 0xFF -> 1
  ippiSet_8u_C1R(0, pDstData, cbDestRow, { width, height });

  const int step = 1;
  for (int r = 1; r < height - 1; r += step) {
    auto srcblock = pSrcData + r * cbSrcRow + cbSrcInc;
    auto destBlock = pDstData + r * cbDestRow + cbDestInc;
    next(srcblock, cbSrcRow, destBlock, cbDestRow, { width - 2, step });
  }
}

void AbstractCpuUniverse::nextIppOmp(ci::Channel &src, ci::Channel &dst) const {
  assert(src.getBounds() == dst.getBounds());

  const auto &bounds = src.getBounds();

  auto pSrcData = src.getData();
  auto cbSrcRow = src.getRowBytes();
  auto cbSrcInc = src.getIncrement();

  auto pDstData = dst.getData();
  auto cbDestRow = dst.getRowBytes();
  auto cbDestInc = dst.getIncrement();

  auto height = bounds.getHeight();
  auto width = bounds.getWidth();

  for (int r = 0; r < height; ++r) {
    *(pSrcData + r *cbSrcRow) = 0x00;
    *(pSrcData + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
  fill(pSrcData + (height - 1) * cbSrcRow,
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0xFF);

  ippiAndC_8u_C1R(pSrcData, cbSrcRow, 1, pSrcData, cbSrcRow,
                  { width, height }); // map 0xFF -> 1
  ippiSet_8u_C1R(0, pDstData, cbDestRow, { width, height });

  const int step = 1;
#pragma omp parallel for
  for (int r = 1; r < height - 1; r += step) {
    auto srcblock = pSrcData + r * cbSrcRow + cbSrcInc;
    auto destBlock = pDstData + r * cbDestRow + cbDestInc;
    next(srcblock, cbSrcRow, destBlock, cbDestRow, { width - 2, step });
  }
}

void AbstractCpuUniverse::nextIppTbb(ci::Channel &src, ci::Channel &dst) const {
#if defined USE_TBB
  assert(src.getBounds() == dst.getBounds());

  const auto &bounds = src.getBounds();

  auto pSrcData = src.getData();
  auto cbSrcRow = src.getRowBytes();
  auto cbSrcInc = src.getIncrement();

  auto pDstData = dst.getData();
  auto cbDestRow = dst.getRowBytes();
  auto cbDestInc = dst.getIncrement();

  auto height = bounds.getHeight();
  auto width = bounds.getWidth();

  for (int r = 0; r < height; ++r) {
    *(pSrcData + r *cbSrcRow) = 0x00;
    *(pSrcData + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
  fill(pSrcData + (height - 1) * cbSrcRow,
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0xFF);

  ippiAndC_8u_C1R(pSrcData, cbSrcRow, 1, pSrcData, cbSrcRow,
                  { width, height }); // map 0xFF -> 1
  ippiSet_8u_C1R(0, pDstData, cbDestRow, { width, height });

// auto block = blocked_range<int>(1, height - 1);
// parallel_for(block, [&](const blocked_range<int> &range) {
//  auto srcblock = src + range.begin() * cbSrcRow + cbSrcInc;
//  auto destBlock = dest + range.begin() * cbDestRow + cbDestInc;
//  next(srcblock, cbSrcRow, destBlock, cbDestRow, { width - 2, range.end() -
// range.begin() });
//});

// auto srcblock = src + cbSrcRow + cbSrcInc;
// auto destblock = dest + cbDestRow + cbDestInc;
// next(srcblock, cbSrcRow, destblock, cbDestRow, { width - 2, height - 2 });
#else
  throw exception("TBB not availible");
#endif // USE_TBB
}

void AbstractCpuUniverse::next(uint8_t *src, int srcStride, uint8_t *dest,
                               int destStride, const ci::Vec2i &_roi) const {
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
    ippiAdd_8u_C1RSfs(ptr, srcStride, dest + offset, destStride, dest + offset,
                      destStride, roi, 0);
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

int AbstractCpuUniverse::width() const { return d->src().getWidth(); }

int AbstractCpuUniverse::height() const { return d->src().getHeight(); }

void AbstractCpuUniverse::add(const Vec2i &p) {
  assert(p.x < width() && p.x >= 0);
  assert(p.y < height() && p.y >= 0);
  d->src().setValue({ p.x, p.y }, 0xFF);
}
