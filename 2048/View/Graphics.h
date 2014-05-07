#pragma once

#include <memory>

class Timer;

class Graphics {
  static std::shared_ptr<Graphics> graphics;

  struct Data;
  std::unique_ptr<Data> d;

public:
  Graphics();
  virtual ~Graphics();

  Graphics(const Graphics&);

  static std::shared_ptr<Graphics> &instance();

  void setup();
  void tearDown();
  void draw();

  virtual float frameInterval() const;
};

inline const std::shared_ptr<Graphics> &gfx() { return Graphics::instance(); }
