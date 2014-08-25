#pragma once

#include <memory>

class Timer;

namespace animation {

class Sprite;

class Gfx {
  static std::shared_ptr<Gfx> graphics;

  struct Data;
  std::unique_ptr<Data> d;

  friend class Sprite;
  void add(Sprite &sprite);
  void remove(const Sprite &sprite);

public:
  Gfx();
  virtual ~Gfx();

  Gfx(const Gfx &) = delete;

  static std::shared_ptr<Gfx> &instance();

  void setup();
  void tearDown();
  void draw();

  virtual float frameInterval() const;
};

inline const std::shared_ptr<Gfx> &gfx() { return Gfx::instance(); }
}