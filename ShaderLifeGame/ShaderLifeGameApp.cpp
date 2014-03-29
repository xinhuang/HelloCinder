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

  init();
}

void ShaderLifeGameApp::init() {
  int width = getWindowWidth();
  int height = getWindowHeight();

  fbos[0] = gl::Fbo(width, height, false);
  fbos[0].bindFramebuffer();
  gl::setViewport(fbos[0].getBounds());
  gl::setMatricesWindow(fbos[0].getSize(), false);
  gl::clear();
  gl::draw(createUniverse(fbos[0].getWidth(), fbos[0].getHeight()));
  fbos[0].unbindFramebuffer();

  fbos[1] = gl::Fbo(width, height, false);
  fbos[1].bindFramebuffer();
  gl::setViewport(fbos[1].getBounds());
  gl::clear();
  fbos[1].unbindFramebuffer();
}

ci::gl::Texture ShaderLifeGameApp::createUniverse(int width, int height) {
  auto surface = Surface(width, height, false);

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

  return gl::Texture(surface);
}

void ShaderLifeGameApp::keyUp(KeyEvent event) {
  switch (event.getCode()) {
  case KeyEvent::KEY_f:
    setFullScreen(!isFullScreen());
    break;

  case KeyEvent::KEY_RETURN:
    init();
    break;

  case KeyEvent::KEY_ESCAPE:
    quit();
    break;
  }
}

void ShaderLifeGameApp::draw() {
  gl::Fbo& src = fbos[ifbo];
  gl::Fbo& dst = fbos[1 - ifbo];

  dst.bindFramebuffer();
    gl::setViewport(dst.getBounds());
    gl::setMatricesWindow(dst.getSize(), false);
    gl::clear();
    gl::color(Color::white());

    src.bindTexture();
      mShader->bind();
        mShader->uniform("pixel",
                         Vec2f::one() / static_cast<Vec2f>(src.getSize()));
        mShader->uniform("src", 0);
        gl::drawSolidRect(dst.getBounds());
      mShader->unbind();
    src.unbindTexture();
  dst.unbindFramebuffer();

  gl::clear(Color::black());
  gl::setViewport(getWindowBounds());
  gl::setMatricesWindow(dst.getSize(), false);

  gl::draw(dst.getTexture(), dst.getBounds());

  ifbo = (ifbo + 1) % 2;
}

CINDER_APP_BASIC(ShaderLifeGameApp, RendererGl)