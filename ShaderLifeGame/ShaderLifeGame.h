#pragma once

#include <cinder/gl/Texture.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/gl/fbo.h>
#include <cinder/app/AppBasic.h>

class ShaderLifeGameApp : public ci::app::AppBasic {
  int ifbo = 0;
  ci::gl::Fbo fbos[2];
  ci::gl::TextureRef mTexture;
  ci::gl::GlslProgRef mShader;

  void init();
  static ci::gl::Texture createUniverse(int width, int height);

public:
  void setup() final;
  void keyUp(ci::app::KeyEvent event) final;

  void draw() final;
};
