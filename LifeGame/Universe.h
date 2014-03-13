#pragma once

#include "Rule.h"

#include <memory>

#include <cinder/gl/Texture.h>

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

  void add(const ci::Vec2i &p, CellState s) {}

  bool isSilent() const { return true; }

  void next(Universe &u);

  bool operator[](const ci::Vec2i &p) const { return false; }
  bool operator[](const Point &p) const { return (*this)[ci::Vec2i(p.x, p.y)]; }

  void add(const Point &p);
  void addn(const Point &p) { add(p); }

  static Universe bigBang(int width, int height);

private:
  Universe(const Universe& u);
  Universe& operator=(Universe& u);

  int width() const;
  int height() const;

  void next(uint8_t* src, int srcStride, uint8_t* dest, int destStride, const ci::Vec2i& roi) const;

  struct Data;
  std::unique_ptr<Data> d;
};

std::ostream &operator<<(std::ostream &os, const Universe &u);
