#include "Clip.h"

using namespace std;

using namespace ci;

struct Clip::Data {
  bool reversed = false;
  float elapsed = 0;
  float duration = 0;
  float delta_alpha = 0.f;
  float alpha = 1.f;
  float scale_to = 1.f;
  float initial_scale = 1.f;
  ci::Vec2f offset;
  shared_ptr<Slice> renderable;
};

Clip::Clip() : d(make_unique<Data>()) {}

Clip::Clip(const std::shared_ptr<Slice> &renderable) : Clip() {
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
  auto elapsed = d->elapsed;
  if (d->reversed)
    elapsed = d->duration - d->elapsed;
  return d->offset * elapsed / d->duration;
}

float Clip::alpha() const {
  assert(d->elapsed <= d->duration);
  auto elapsed = d->elapsed;
  if (d->reversed)
    elapsed = d->duration - d->elapsed;
  return d->alpha + d->delta_alpha * elapsed / d->duration;
}

float Clip::scale() const {
  assert(d->elapsed <= d->duration);
  auto elapsed = d->elapsed;
  if (d->reversed)
    elapsed = d->duration - d->elapsed;
  return d->initial_scale +
         (d->scale_to - d->initial_scale) * elapsed / d->duration;
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

Clip &Clip::scale(float begin, float end) {
  d->initial_scale = begin;
  d->scale_to = end;
  return *this;
}

Clip &Clip::duration(float seconds) {
  d->duration = seconds;
  return *this;
}

Clip &Clip::reverse(bool reversed) {
  d->reversed = reversed;
  return *this;
}

void Clip::draw(float elapsed, ci::Rectf rect) {
  d->elapsed = elapsed;

  if (finished() || !d->renderable)
    return;

  auto scale_factor = scale();
  if (scale_factor != 1.f) {
    auto size = rect.getSize() * scale_factor;
    auto delta = (rect.getSize() - size) / 2;
    rect = { rect.getUpperLeft() + delta, rect.getUpperLeft() + size + delta };
  }
  rect += offset();
  d->renderable->draw(rect, alpha());
  d->elapsed = NAN;
}
