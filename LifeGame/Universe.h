#pragma once

#include <cinder/gl/Texture.h>

#include <memory>
#include <iosfwd>

struct Point;

template <typename T>
class UniverseWrapper {
  std::unique_ptr<T> u_;
public:
  UniverseWrapper() : u_(std::make_unique<T>()) {}
  UniverseWrapper(int width, int height) : u_(std::make_unique<T>(width, height)) {}

  UniverseWrapper& operator=(T&& v) {
    u_ = std::make_unique<T>(std::forward<T>(v));
    return *this;
  }

  void next() { u_->next(); }
  void add(const ci::Vec2i &p) { u_->add(p); }
  int width() const { return u_->width(); }
  int height() const { return u_->height(); }
  int size() const { return u_->size(); }
  ci::gl::Texture render() const { return u_->render(); }
};

class Universe {
public:
  Universe();
  Universe(int width, int height);
  Universe(Universe &&u);
  Universe &operator=(Universe &&u);

  ~Universe();

  int size() const;

  ci::gl::Texture texture() const ;
  ci::gl::Texture render() const { return texture(); }

  void next();

  void add(const ci::Vec2i &p);

  int width() const;
  int height() const;


private:
  void next(ci::Channel &src, ci::Channel &dst) const;
  void nextLoop(ci::Channel &src, ci::Channel &dst) const;
  void nextLoopOmp(ci::Channel &src, ci::Channel &dst) const;
  void nextIpp(ci::Channel &src, ci::Channel &dst) const;
  void nextIppTbb(ci::Channel &src, ci::Channel &dst) const;
  void nextIppOmp(ci::Channel &src, ci::Channel &dst) const;
  void next(uint8_t *src, int srcStride, uint8_t *dest, int destStride,
            const ci::Vec2i &roi) const;

  struct Data;
  std::unique_ptr<Data> d;
};

template <typename T = Universe>
T bigBang(int width, int height) {
  T u(width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = Random::next<int>(width - 1);
    int y = Random::next<int>(height - 1);
    u.add({ x, y });
  }
  return u;
}
