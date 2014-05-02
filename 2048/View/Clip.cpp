#include "Clip.h"

using namespace std;

using namespace ci;

struct Clip::Data {
  float elapsed = 0;
  float duration = 0;
  float delta_alpha = 0.f;
  float alpha = 1.f;
  float scale = 0.f;
  float initial_scale = 1.f;
  ci::Vec2f offset;
  shared_ptr<IRenderable> renderable;
};

Clip::Clip() : d(make_unique<Data>()) {}

Clip::Clip(const std::shared_ptr<IRenderable> &renderable) : Clip() {
  d->renderable = renderable;
}

Clip::Clip(const Clip &anim) : Clip() { *d = *(anim.d); }

Clip::~Clip() {}

Clip &Clip::operator=(const Clip &anim) {
  *d = *(anim.d);
  return *this;
}

ci::Vec2f Clip::offset() const {
  assert(d->elapsed <= d->duration);
  return d->offset * d->elapsed / d->duration;
}

float Clip::alpha() const {
  assert(d->elapsed <= d->duration);
  return d->alpha + d->delta_alpha * d->elapsed / d->duration;
}

float Clip::scale() const {
  assert(d->elapsed <= d->duration);
  return d->initial_scale + d->scale * d->elapsed / d->duration;
}

float Clip::duration() const { return d->duration; }

bool Clip::finished() const { return d->elapsed > d->duration; }

Clip &Clip::moveby(const ci::Vec2f &offset) {
  d->offset = offset;
  return *this;
}

Clip &Clip::fadeby(float delta, float from) {
  d->delta_alpha = delta;
  d->alpha = from;
  return *this;
}

Clip &Clip::scaleby(float scale, float from) {
  d->scale = scale;
  d->initial_scale = from;
  return *this;
}

Clip &Clip::duration(float seconds) {
  d->duration = seconds;
  return *this;
}

void Clip::update(float elapsed_seconds) {
  d->elapsed += elapsed_seconds;
}

void Clip::rewind() {
  d->elapsed = 0.f;
}

void Clip::draw(ci::Rectf rect) {
  if (finished() || !d->renderable)
    return;

  auto scale_factor = scale();
  if (scale_factor != 1.f) {
    auto size = rect.getSize() * scale_factor;
    rect = { rect.getUpperLeft(), rect.getUpperLeft() + size };
  }
  rect += offset();
  d->renderable->draw(rect, alpha());
}

void Clip::draw(float elapsed, ci::Rectf rect) {
  float old_elapsed = d->elapsed;
  d->elapsed = elapsed;

  if (finished() || !d->renderable)
    return;

  auto scale_factor = scale();
  if (scale_factor != 1.f) {
    auto size = rect.getSize() * scale_factor;
    rect = { rect.getUpperLeft(), rect.getUpperLeft() + size };
  }
  rect += offset();
  d->renderable->draw(rect, alpha());
  d->elapsed = old_elapsed;
}
