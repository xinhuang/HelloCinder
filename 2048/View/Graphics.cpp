#include "Graphics.h"

#include "Timer.h"
#include "Animation.h"

using namespace std;

std::shared_ptr<Graphics> Graphics::graphics = shared_ptr<Graphics>(new Graphics());

struct Graphics::Data {
  Timer timer;
};

Graphics::Graphics() : d(make_unique<Data>()) {}

Graphics::~Graphics() {}

Graphics::Graphics(const Graphics&) {
  int a = 0;
}

shared_ptr<Graphics> &Graphics::instance() {
  return graphics; 
}

void Graphics::setup() { d->timer.reset(); }

void Graphics::tearDown() {}

void Graphics::draw() {
  d->timer.reset();
}

float Graphics::frameInterval() const { return (float)d->timer.elapsed(); }
