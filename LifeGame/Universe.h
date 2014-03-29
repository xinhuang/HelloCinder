#pragma once


#include <cinder/gl/Texture.h>

#include <memory>
#include <iosfwd>

struct Point;

class Universe {
public:
  Universe();
  Universe(int width, int height);
  Universe(Universe &&u);
  Universe &operator=(Universe &&u);

  ~Universe();

  int size() const;

  ci::gl::Texture texture();

  void next();

  void add(const ci::Vec2i &p);

  int width() const;
  int height() const;

  static Universe bigBang(int width, int height);

private:
  void next(ci::Channel& src, ci::Channel &dst) const;
  void next(uint8_t* src, int srcStride, uint8_t* dest, int destStride, const ci::Vec2i& roi) const;

  struct Data;
  std::unique_ptr<Data> d;
};

std::ostream &operator<<(std::ostream &os, const Universe &u);
