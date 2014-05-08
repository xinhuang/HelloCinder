#include "Sprite.h"

#include <cinder/Rect.h>

using namespace ci;

#include <map>

using namespace std;

struct Sprite::Data {
  map<int, Animation> layers;
};

Sprite::Sprite() : d(make_unique<Data>()) {}

Sprite::Sprite(const Sprite& sprite) : Sprite() {
  *d = *(sprite.d);
}

Sprite::~Sprite() {}

Sprite::Sprite(initializer_list<Layer> layers) : Sprite() {
  d->layers = move(layers);
}

Sprite& Sprite::operator=(const Sprite& sprite) {
  *d = *(sprite.d);
  return *this;
}

Animation& Sprite::layer(int i) {
  return d->layers[i];
}

void Sprite::draw(const Rectf& rect) {
  for (auto& layer : d->layers) {
    get<1>(layer).draw(rect);
  }
}

void Sprite::draw() {

}
