#pragma once

#include <memory>

#include <cinder/Vector.h>

class Environment {
  struct Data;
  std::unique_ptr<Data> d;

  Environment();

public:
  ~Environment();

  static Environment& instance() {
    static Environment env;
    return env;
  }

  const ci::Vec2f& cellSize() const;
  void setCellSize(const ci::Vec2f& size);
};
