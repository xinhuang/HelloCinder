#include "Animation.h"

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

Animation::Animation(const Animation &other) : Animation() {
  *this = other;
}

Animation &Animation::operator=(const Animation &other) {
  d->iframe = other.d->iframe;
  d->frames = other.d->frames;
  d->animations = other.d->animations;
  return *this;
}

Animation::~Animation() {}

void Animation::draw(const ci::Rectf &rect) {
  for_each(d->animations.begin(), d->animations.end(),
           [&](Animation& anim) { anim.draw(rect); });
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

Animation operator+(Animation sum, const Animation &rhs) {
  return sum += rhs;
}

Animation& operator+=(Animation& anim, const Animation &value) {
  anim.d->frames.insert(anim.d->frames.end(), value.d->frames.begin(),
    value.d->frames.end());
  return anim;
}

Animation operator*(Animation product, const Animation &rhs) {
  return product *= rhs;
}

Animation& operator*=(Animation& anim, const Animation &value) {
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

Animation moveBy(ci::gl::TextureRef &tex, const ci::Vec2f &offset,
                  int nframe) {
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
