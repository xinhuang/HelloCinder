#pragma once

#include <cinder/gl/Texture.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/gl/fbo.h>
#include <cinder/app/AppBasic.h>

class ShaderLifeGameApp : public ci::app::AppBasic {
public:
  void setup();
  void keyDown(ci::app::KeyEvent event);

  void draw();

  ci::gl::Fbo ping;
  ci::gl::Fbo pong;
  ci::gl::TextureRef mTexture;
  ci::gl::GlslProgRef mShader;
};
