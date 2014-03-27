#include "ShaderLifeGame.h"

#include <algorithm>

using namespace std;

#include "Resources.h"

using namespace ci;
using namespace ci::app;

void ShaderLifeGameApp::setup() {
  try {
    mShader = gl::GlslProg::create(loadResource(RES_PASSTHRU_VERT),
                                   loadResource(RES_LIFEGAME_FRAG));
  }
  catch (gl::GlslProgCompileExc &exc) {
    std::cout << "Shader compile error: " << std::endl;
    std::cout << exc.what();
  }
  catch (...) {
    std::cout << "Unable to load shader" << std::endl;
  }

  ping = gl::Fbo(getWindowWidth(), getWindowHeight(), false);
  pong = gl::Fbo(ping.getWidth(), ping.getHeight(), false);

  ping.bindFramebuffer();
  gl::setViewport(ping.getBounds());
  gl::clear();
  ping.unbindFramebuffer();

  pong.bindFramebuffer();
  gl::setViewport(pong.getBounds());
  gl::clear();
  pong.unbindFramebuffer();
}

void ShaderLifeGameApp::keyDown(KeyEvent event) {
  if (event.getCode() == app::KeyEvent::KEY_f) {
    setFullScreen(!isFullScreen());
  }
}

void ShaderLifeGameApp::draw() {
  static bool initialized = false;

  if (!initialized) {
    initialized = true;
    auto surface = Surface(ping.getWidth(), ping.getHeight(), false);

    srand(time(nullptr));
    auto iter = surface.getIter();
    int r = 0;
    while (iter.line()) {
      ++r;
      while (iter.pixel()) {
        if (rand() % 100 < 70) {
          iter.r() = 0x00;
          iter.g() = 0x00;
          iter.b() = 0x00;
        } else {
          iter.r() = 0xFF;
          iter.g() = 0xFF;
          iter.b() = 0xFF;
        }
      }
    }

    ping.bindFramebuffer();
    gl::setViewport(ping.getBounds());
    gl::setMatricesWindow(pong.getSize(), false);
    gl::draw(gl::Texture(surface));
    ping.unbindFramebuffer();

    gl::setViewport(ping.getBounds());
    gl::setMatricesWindow(ping.getSize());
    gl::draw(ping.getTexture(), ping.getBounds());
    return;
  }
  {
    pong.bindFramebuffer();
    gl::setViewport(pong.getBounds());
    gl::setMatricesWindow(pong.getSize());
    gl::clear();
    gl::color(Color::white());
    ping.bindTexture();

    mShader->bind();
    mShader->uniform("pixel",
                     Vec2f::one() / static_cast<Vec2f>(pong.getSize()));
    mShader->uniform("src", 0);
    gl::drawSolidRect(pong.getBounds());

    mShader->unbind();
    ping.unbindTexture();
    pong.unbindFramebuffer();
  }

  gl::clear(Color::black());
  gl::setViewport(pong.getBounds());
  gl::setMatricesWindow(pong.getSize(), false);

  gl::draw(pong.getTexture(), pong.getBounds());

  swap(ping, pong);
}

CINDER_APP_BASIC(ShaderLifeGameApp, RendererGl)