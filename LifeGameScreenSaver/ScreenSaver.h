#pragma once

#include <cinder/Cinder.h>
#include <cinder/app/AppScreenSaver.h>
#include <cinder/gl/TextureFont.h>

#include <chrono>

#include "../LifeGame/Universe.h"
#include "../LifeGame/GpuGlslUniverse.h"
#include "../LifeGame/Sysinfo.h"

class ScreenSaver : public ci::app::AppScreenSaver {
public:
  ScreenSaver();

  void setup() final;
  void draw() final;
  void update() final;

private:
  const std::chrono::seconds REFRESH_TIME = { 300 };
  typedef GpuGlslUniverse universe_t;

  Sysinfo sysinfo_;
  ci::gl::TextureFontRef font_;
  std::unique_ptr<IUniverse> u_;
};
