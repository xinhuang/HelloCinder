#include "Graphics.h"

#include "Timer.h"

using namespace std;

Graphics Graphics::graphics;

struct Graphics::Data {
  Timer timer;
};

Graphics::Graphics() : d(make_unique<Data>()) {}

Graphics::~Graphics() {}

Graphics& Graphics::instance() {
  return graphics;
}

void Graphics::setup() {
  d->timer.reset();
}

void Graphics::tearDown() {}

void Graphics::draw() {
  d->timer.reset();
}
