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
#include "ivlib.h"

#ifdef USE_TBB
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace tbb;
#endif // USE_TBB

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
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0x00);

  static const int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
  static const int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };
  for (int r = 1; r < height - 1; ++r) {
    for (int c = 1; c < width - 1; ++c) {
      auto pixel = pDstData + r * cbDestRow + cbDestInc * c;
      auto srcpixel = pSrcData + r * cbSrcRow + cbSrcInc * c;
      int count = 0;
      for (int i = 0; i < 8; ++i) {
        auto ptr = srcpixel + x[i] * cbSrcInc + y[i] * cbSrcRow;
        if (*ptr == ALIVE_COLOR)
          ++count;
      }

      switch (count) {
      case 3:
        *pixel = ALIVE_COLOR;
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
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0x00);

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
        *pixel = ALIVE_COLOR;
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
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0x00);

  ippiAndC_8u_C1R(pSrcData, cbSrcRow, 1, pSrcData, cbSrcRow,
                  { width, height }); // map ALIVE_COLOR -> 1
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
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0x00);

  ippiAndC_8u_C1R(pSrcData, cbSrcRow, 1, pSrcData, cbSrcRow,
                  { width, height }); // map ALIVE_COLOR -> 1
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
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc,
       ALIVE_COLOR);

  ippiAndC_8u_C1R(pSrcData, cbSrcRow, 1, pSrcData, cbSrcRow,
                  { width, height }); // map ALIVE_COLOR -> 1
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

void CpuAvxUniverse::next(ci::Channel &src, ci::Channel &dst) const {
  next(src.getWidth(), src.getHeight(), src.getData(), src.getRowBytes(),
       src.getIncrement(), dst.getData(), dst.getRowBytes(),
       dst.getIncrement());
}

void CpuAvxUniverse::next(int width, int height, uint8_t *src, int srcStride,
                          int srcInc, uint8_t *dst, int dstStride,
                          int dstInc) const {
  for (int r = 0; r < height; ++r) {
    *(src + r * srcStride) = 0x00;
    *(src + r * srcStride + (width - 1) * srcInc) = 0x00;
  }
  fill(src, src + (width - 1) * srcInc, 0x00);
  fill(src + (height - 1) * srcStride,
       src + (height - 1) * srcStride + (width - 1) * srcInc, 0x00);

  for (int r = 0; r < height; ++r)
    ui8vandcu(width, src + srcStride * r, src + srcStride * r, 1);

  const int x[] = {
      -1, 0, 1, -1, 1, -1, 0, 1,
  };
  const int y[] = {
      -1, -1, -1, 0, 0, 1, 1, 1,
  };

  for (int r = 1; r < height - 1; ++r) {
    uint8_t *row = src + srcStride * r + srcInc;
    uint8_t *dst_row = dst + dstStride * r + dstInc;
    memcpy(sum, row + y[0] * srcStride + x[0] * srcInc, width - 2);
    for (int i = 1; i < 8; ++i)
		ui8vaddu(width - 2, sum, row + y[i] * srcStride + x[i] * srcInc, sum);

    memcpy(dst_row, sum, width - 2);

    ui8vcmpcu(width - 2, sum, sum, 2, ivEq);		// 2 => 255
    ui8vandu(width - 2, sum, row, sum);				// 255 & 1
	ui8vcmpcu(width - 2, sum, sum, 1, ivEq);		// 1 => 255

    ui8vcmpcu(width - 2, dst_row, dst_row, 3, ivEq);	// 3 => 255

    ui8voru(width - 2, sum, dst_row, dst_row);
  }
}

void AbstractCpuUniverse::nextAvxOmp(ci::Channel &src, ci::Channel &dst) const {
  const int width = src.getWidth();
  const int height = src.getHeight();

  auto pSrcData = src.getData();
  auto cbSrcRow = src.getRowBytes();
  auto cbSrcInc = src.getIncrement();

  auto pDstData = dst.getData();
  auto cbDestRow = dst.getRowBytes();
  auto cbDestInc = dst.getIncrement();

  for (int r = 0; r < height; ++r) {
    *(pSrcData + r *cbSrcRow) = 0x00;
    *(pSrcData + r *cbSrcRow + (width - 1) *cbSrcInc) = 0x00;
  }
  fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
  fill(pSrcData + (height - 1) * cbSrcRow,
       pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc, 0x00);

  for (int r = 0; r < height; ++r)
    ui8vandcu(width, src.getData() + src.getRowBytes() * r,
              src.getData() + src.getRowBytes() * r, 1);

  const int x[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
  const int y[] = { 0, 1, 1, 1, 0, -1, -1, -1, };

#pragma omp parallel for
  for (int r = 1; r < height - 1; ++r) {
    auto sum = aligned_malloc<uint8_t>(width - 2, 32);
    uint8_t *row = src.getData() + src.getRowBytes() * r + src.getIncrement();
    uint8_t *dst_row =
        dst.getData() + dst.getRowBytes() * r + dst.getIncrement();
    memcpy(sum, row + y[0] * src.getRowBytes() + x[0] * src.getIncrement(),
           width - 2);
    for (int i = 1; i < 8; ++i)
      ui8vaddu(width - 2, sum,
               row + y[i] * src.getRowBytes() + x[i] * src.getIncrement(), sum);

    memcpy(dst_row, sum, width - 2);

    ui8vcmpcu(width - 2, sum, sum, 2, ivEq);
    ui8vandu(width - 2, sum, row, sum);

    ui8vcmpcu(width - 2, dst_row, dst_row, 3, ivEq);

    ui8voru(width - 2, sum, dst_row, dst_row);
    aligned_free(sum);
  }
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
  d->src().setValue({ p.x, p.y }, ALIVE_COLOR);
}
