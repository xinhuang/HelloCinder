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

  bool isSilent() const { return true; }

  void next(Universe &u);

  void add(const ci::Vec2i &p);

  static Universe bigBang(int width, int height);

private:
  Universe(const Universe& u);
  Universe& operator=(Universe& u);

  int width() const;
  int height() const;

  struct Data;
  std::unique_ptr<Data> d;
};

std::ostream &operator<<(std::ostream &os, const Universe &u);
