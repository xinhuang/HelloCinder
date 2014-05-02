#include "Animation.h"

#include "TextureRenderer.h"

#include <cinder/gl/Texture.h>

using namespace ci;

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

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

// ---------------------------------------------- //

struct Animation2::Data {
  static unique_ptr<Timer> timer;

  bool cyclic = false;
  float elapsed = 0;
  vector<Clip> clips;
};

unique_ptr<Timer> Animation2::Data::timer = make_unique<Timer>();

Animation2::Animation2() : d(make_unique<Data>()) {}

Animation2::~Animation2() {}

Animation2::Animation2(const Animation2& anim) : Animation2() {
  *this = anim;
}

Animation2::Animation2(const std::initializer_list<Clip> &clips)
    : Animation2() {
  d->clips = clips;
}

Animation2& Animation2::operator=(const Animation2& anim) {
  *d = *(anim.d);
  return *this;
}

void Animation2::rewind() {
  for (auto& clip : d->clips)
    clip.rewind();
}

Animation2& Animation2::cyclic(bool value) {
  d->cyclic = value;
  return *this;
}

void Animation2::draw(const ci::Rectf &rect) {
  float frame_interval = (float)Data::timer->elapsed();
  d->elapsed += frame_interval;

  do {   
    float elapsed = d->elapsed;
    for (auto &clip : d->clips) {
      if (elapsed <= clip.duration()) {
        clip.update(frame_interval);
        clip.draw(rect);
        return;
      } else {
        elapsed -= clip.duration();
      }
    }
    if (d->cyclic) {
      d->elapsed = fmod(d->elapsed, duration());
      rewind();
    }
  } while (d->cyclic);
}

float Animation2::duration() const {
  return accumulate(d->clips.begin(), d->clips.end(), 0.f,
                    [&](float v, const Clip &c) { return v + c.duration(); });
}

void Animation2::setTimer(Timer *timer) {
  Data::timer.release();
  Data::timer.reset(timer);
}

Timer *Animation2::timer() { return Data::timer.get(); }

Animation2 operator+(Animation2 lhs, const Animation2 &rhs) {
  lhs.d->clips.insert(lhs.d->clips.end(), rhs.d->clips.begin(),
                      rhs.d->clips.end());
  return lhs;
}

// ----------------------------------------------------------------- //

#include <map>

struct Sprite::Data {
  map<int, Animation2> layers;
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

Animation2& Sprite::layer(int i) {
  return d->layers[i];
}

void Sprite::draw(const Rectf& rect) {
  for (auto& layer : d->layers) {
    get<1>(layer).draw(rect);
  }
}
