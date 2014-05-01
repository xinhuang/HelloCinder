#include "Animation.h"

#include "TextureRenderer.h"

#include <cinder/gl/Texture.h>

using namespace ci;

#include <vector>
#include <algorithm>

using namespace std;

struct Animation::Data {
  size_t iframe = 0;
  vector<shared_ptr<IRenderable> > frames;
  vector<Animation> animations;
};

Animation::Animation() : d(make_unique<Data>()) {}

Animation::Animation(unique_ptr<IRenderable> &&frame) : Animation() {
  (*this) += move(frame);
}

Animation::Animation(const Animation &other) : Animation() { *this = other; }

Animation &Animation::operator=(const Animation &other) {
  d->iframe = other.d->iframe;
  d->frames = other.d->frames;
  d->animations = other.d->animations;
  return *this;
}

Animation::~Animation() {}

void Animation::draw(const ci::Rectf &rect) {
  for_each(d->animations.begin(), d->animations.end(),
           [&](Animation &anim) { anim.draw(rect); });
  if (d->frames.size() == 0)
    return;
  if (d->iframe < d->frames.size()) {
    d->frames[d->iframe]->draw(rect);
    d->iframe += 1;
  } else {
    d->frames.back()->draw(rect);
  }
}

Animation &Animation::reverse() {
  std::reverse(begin(d->frames), end(d->frames));
  return *this;
}

Animation &operator+=(Animation &anim, std::shared_ptr<IRenderable> &&value) {
  anim.d->frames.push_back(move(value));
  return anim;
}

Animation operator+(Animation sum, const Animation &rhs) { return sum += rhs; }

Animation &operator+=(Animation &anim, const Animation &value) {
  anim.d->frames.insert(anim.d->frames.end(), value.d->frames.begin(),
                        value.d->frames.end());
  return anim;
}

Animation operator*(Animation product, const Animation &rhs) {
  return product *= rhs;
}

Animation &operator*=(Animation &anim, const Animation &value) {
  anim.d->animations.push_back(value);
  return anim;
}

Animation scaleBy(ci::gl::TextureRef &tex, float from, float to, int nframe) {
  Animation anim;
  float scale = from;
  float stepScale = (to - from) / nframe;
  for (int i = 0; i < nframe - 1; ++i) {
    anim += make_unique<TextureRenderer>(tex, scale);
    scale += stepScale;
  }
  anim += make_unique<TextureRenderer>(tex, to);

  return anim;
}

Animation moveBy(ci::gl::TextureRef &tex, const ci::Vec2f &offset, int nframe) {
  Animation anim;
  Vec2f step = offset / static_cast<float>(nframe);
  for (int i = 0; i < nframe - 1; ++i) {
    anim += make_unique<TextureRenderer>(tex, step * i);
  }
  anim += make_unique<TextureRenderer>(tex, offset);

  return anim;
}

Animation fade(ci::gl::TextureRef &tex, float begin, float end, int nframe) {
  Animation anim;
  auto step = (end - begin) / static_cast<float>(nframe);
  for (int i = 0; i < nframe - 1; ++i) {
    anim += TextureRenderer::fade(tex, begin + step * i);
  }
  anim += TextureRenderer::fade(tex, end);

  return anim;
}

// ------------------------------------------------------------------ //

struct Clip::Data {
  static unique_ptr<Timer> timer;

  float passed = 0;
  float duration;
  float alpha = 0.f;
  float scale = 0.f;
  ci::Vec2f offset;
  shared_ptr<IRenderable> renderable;
};

std::unique_ptr<Timer> Clip::Data::timer = make_unique<Timer>();

Clip::Clip() : d(make_unique<Data>()) {}

Clip::Clip(const std::shared_ptr<IRenderable> &renderable)
    : Clip() {
  d->renderable = renderable;
}

Clip::Clip(const Clip &anim) : Clip() {
  *d = *(anim.d);
}

Clip::~Clip() {}

Clip &Clip::operator=(const Clip &anim) {
  *d = *(anim.d);
  return *this;
}

ci::Vec2f Clip::offset() const {
  auto elapsed = (float)Data::timer->elapsed() + d->passed;
  assert(elapsed <= d->duration);
  return d->offset * elapsed / d->duration;
}

float Clip::alpha() const {
  auto elapsed = (float)Data::timer->elapsed();
  assert(elapsed <= d->duration);
  return 1.f + d->alpha * elapsed / d->duration;
}

float Clip::scale() const {
  auto elapsed = (float)Data::timer->elapsed();
  assert(elapsed <= d->duration);
  return 1.f + d->scale * elapsed / d->duration;
}

Clip &Clip::moveby(const ci::Vec2f &offset) {
  d->offset = offset;
  return *this;
}

Clip &Clip::fadeby(float delta) {
  d->alpha = delta;
  return *this;
}

Clip &Clip::scaleby(float scale) {
  d->scale = scale;
  return *this;
}

Clip &Clip::duration(float seconds) {
  d->duration = seconds;
  return *this;
}

void Clip::draw(ci::Rectf rect) {
  auto elapsed = (float)Data::timer->elapsed();
  if (elapsed > d->duration) {
    return;
  }
  auto scale_factor = scale();
  if (scale_factor != 1.f) {
    auto size = rect.getSize() * scale_factor;
    rect = { rect.getUpperLeft(), rect.getUpperLeft() + size };
  }
  rect += offset();
  d->renderable->draw(rect, alpha());
  d->passed += elapsed;
}

void Clip::setTimer(Timer *timer) {
  Data::timer.release();
  Data::timer.reset(timer);
}

Timer *Clip::timer() { return Data::timer.release(); }

// ---------------------------------------------- //

struct Animation2::Data {
  vector<Clip> clips;
};

Animation2::Animation2() : d(make_unique<Data>()) {}

Animation2::~Animation2() {}

Animation2::Animation2(const std::initializer_list<Clip>& clips): Animation2() {
  d->clips = clips;
}

void Animation2::draw(const ci::Rectf& rect) {
  d->clips[0].draw(rect);
}
