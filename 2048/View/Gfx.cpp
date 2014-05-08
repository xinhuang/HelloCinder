#include "Gfx.h"

#include "Timer.h"
#include "Sprite.h"

#include <vector>
#include <algorithm>

using namespace std;

std::shared_ptr<Gfx> Gfx::graphics = shared_ptr<Gfx>(new Gfx());

struct Gfx::Data {
  Timer timer;
  vector<Sprite*> sprites;
};

Gfx::Gfx() : d(make_unique<Data>()) {}

Gfx::~Gfx() {}

Gfx::Gfx(const Gfx&) {
  int a = 0;
}

shared_ptr<Gfx> &Gfx::instance() {
  return graphics; 
}

void Gfx::setup() { d->timer.reset(); }

void Gfx::tearDown() {}

void Gfx::draw() {
  d->timer.reset();
  for_each(d->sprites.begin(), d->sprites.end(), [&](Sprite *s) { s->draw(); });
}

void Gfx::add(Sprite& sprite) {
  d->sprites.push_back(&sprite);
}

void Gfx::remove(const Sprite& sprite) {
  d->sprites.erase(remove_if(d->sprites.begin(), d->sprites.end(),
                             [&](const Sprite *s) { return s == &sprite; }));
}

float Gfx::frameInterval() const { return (float)d->timer.elapsed(); }
