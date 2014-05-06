#pragma once

#include <memory>

class Graphics {
  static Graphics graphics;

  struct Data;
  std::unique_ptr<Data> d;

  Graphics();

public:
  ~Graphics();

  static Graphics &instance();

  void setup();
  void tearDown();
  void draw();
};

// TODO: remove global variable
inline Graphics &gfx() { return Graphics::instance(); }
