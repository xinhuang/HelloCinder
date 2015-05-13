#pragma once

#include "GameConfig.h"
#include "Random.h"

#include <cinder/gl/Texture.h>

#include <memory>
#include <iosfwd>

#include "ivlib.h"

struct Point;

class IUniverse {
public:
  virtual ~IUniverse() {}
  virtual std::string name() const = 0;
  virtual void next() = 0;
  virtual void add(const ci::Vec2i &p) = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual int size() const = 0;
  virtual ci::gl::Texture render() const = 0;

  const static uint8_t ALIVE_COLOR = 0xFF;
};

template <typename T> std::unique_ptr<T> bigBang(int width, int height) {
  std::unique_ptr<T> u = std::make_unique<T>(width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = Random::next<int>(width - 1);
    int y = Random::next<int>(height - 1);
    u->add({ x, y });
  }
  return u;
}

class AbstractCpuUniverse : public IUniverse {
public:
  AbstractCpuUniverse();
  AbstractCpuUniverse(int width, int height);
  AbstractCpuUniverse(AbstractCpuUniverse &&u);
  AbstractCpuUniverse &operator=(AbstractCpuUniverse &&u);

  ~AbstractCpuUniverse() override;

  int size() const final;

  ci::gl::Texture render() const final { return texture(); }

  void next() final;

  void add(const ci::Vec2i &p) final;

  int width() const final;
  int height() const final;

protected:
  virtual void next(ci::Channel &src, ci::Channel &dst) const = 0;

  ci::gl::Texture texture() const;
  void nextLoop(ci::Channel &src, ci::Channel &dst) const;
  void nextLoopOmp(ci::Channel &src, ci::Channel &dst) const;
  void nextIpp(ci::Channel &src, ci::Channel &dst) const;
  void nextIppTbb(ci::Channel &src, ci::Channel &dst) const;
  void nextIppOmp(ci::Channel &src, ci::Channel &dst) const;
  void nextAvx(ci::Channel &src, ci::Channel &dst) const;
  void nextAvxOmp(ci::Channel &src, ci::Channel &dst) const;
  void next(uint8_t *src, int srcStride, uint8_t *dest, int destStride,
            const ci::Vec2i &roi) const;

  struct Data;
  std::unique_ptr<Data> d;
};

class CpuLoopUniverse : public AbstractCpuUniverse {
public:
  CpuLoopUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final {
    nextLoop(src, dst);
  }
};

class CpuLoopOmpUniverse : public AbstractCpuUniverse {
public:
  CpuLoopOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop with OpenMP"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final {
    nextLoopOmp(src, dst);
  }
};

class CpuIppUniverse : public AbstractCpuUniverse {
public:
  CpuIppUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final {
    nextIpp(src, dst);
  }
};

class CpuIppOmpUniverse : public AbstractCpuUniverse {
public:
  CpuIppOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with OpenMP"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final {
    nextIppOmp(src, dst);
  }
};

class CpuIppTbbUniverse : public AbstractCpuUniverse {
public:
  CpuIppTbbUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with TBB"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final {
    nextIppTbb(src, dst);
  }
};

class CpuAvxUniverse : public AbstractCpuUniverse {
public:
  CpuAvxUniverse(int width, int height) : AbstractCpuUniverse(width, height) {
    sum = aligned_malloc<uint8_t>(width - 2, 32);
  }
  ~CpuAvxUniverse() final { aligned_free(sum); }

  std::string name() const { return "CPU AVX"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final;

private:
  uint8_t *sum;
};

class CpuAvxOmpUniverse : public AbstractCpuUniverse {
public:
  CpuAvxOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU AVX OpenMP"; }

protected:
  void next(ci::Channel &src, ci::Channel &dst) const final {
    nextAvxOmp(src, dst);
  }
};
