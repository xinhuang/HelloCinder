#pragma once

#include <cinder/app/AppBasic.h>
#include <memory>

class Hct : public ::cinder::app::AppBasic {
public:
  Hct();
  ~Hct();

  void setup() final;
  void shutdown() final;

  void draw() final;
  void update() final;

private:
  struct Impl;
  std::unique_ptr<Impl> m;
};
