#include "Sprite.h"

#include "Gfx.h"

#include <cinder/Rect.h>

using namespace ci;

#include <string>
#include <map>

using namespace std;

struct Sprite::Data {
#ifdef _DEBUG
  string name;
#endif
  int depth = 0;
  Rectf rect;
  map<int, Animation> layers;
};

Sprite::Sprite() : d(make_unique<Data>()) { gfx()->add(*this); }

Sprite::Sprite(const Sprite &sprite) : Sprite() { *d = *(sprite.d); }

Sprite::~Sprite() { gfx()->remove(*this); }

Sprite::Sprite(initializer_list<Layer> layers) : Sprite() {
  d->layers = move(layers);
}

Sprite &Sprite::operator=(const Sprite &sprite) {
  *d = *(sprite.d);
  return *this;
}

#ifdef _DEBUG
void Sprite::setName(const string &name) { d->name = name; }
#endif

int Sprite::depth() const { return d->depth; }

void Sprite::setDepth(int value) { d->depth = value; }

int Sprite::layers() const { return d->layers.size(); }

Animation &Sprite::layer(int i) { return d->layers[i]; }

const Animation &Sprite::layer(int i) const { return d->layers[i]; }

const ci::Rectf &Sprite::rect() const { return d->rect; }

void Sprite::setRect(const Rectf &rect) { d->rect = rect; }

void Sprite::draw(const Rectf &rect) {
  for (auto &layer : d->layers) {
    get<1>(layer).draw(rect);
  }
}

void Sprite::draw() { draw(d->rect); }
