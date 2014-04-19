#include "Animation.h"

using namespace ci;

#include <vector>
#include <algorithm>

using namespace std;

struct Animation::Data {
  size_t iframe = 0;
  vector<unique_ptr<IRenderable> > frames;
};

Animation::Animation() : d(make_unique<Data>()) {}

Animation::~Animation() {}

Animation::Animation(Animation &&other) { d = move(other.d); }

Animation &Animation::operator=(Animation &&other) {
  d = move(other.d);
  return *this;
}

Animation &&Animation::zoom(gl::TextureRef &tex, float beginScale,
                            float endScale, int nframes) {

  float scale = beginScale;
  float stepScale = (endScale - beginScale) / nframes;
  for (int i = 0; i < nframes - 1; ++i) {
    auto f = unique_ptr<IRenderable>(new TextureRenderer(tex, scale));
    d->frames.push_back(move(f));
    scale += stepScale;
  }
  auto f = unique_ptr<IRenderable>(new TextureRenderer(tex, endScale));
  d->frames.push_back(move(f));

  return move(*this);
}

Animation &&Animation::reverse() {
  std::reverse(begin(d->frames), end(d->frames));
  return move(*this);
}

void Animation::draw(const ci::Rectf &rect) {
  if (d->frames.size() == 0)
    return;
  if (d->iframe < d->frames.size()) {
    d->frames[d->iframe]->draw(rect);
    d->iframe += 1;
  } else {
    d->frames.back()->draw(rect);
  }
}

Animation &&Animation::moveBy(ci::gl::TextureRef &tex, const ci::Vec2f &offset,
                              int nframes) {
  Vec2f step = offset / static_cast<float>(nframes);
  for (int i = 0; i < nframes - 1; ++i) {
    auto f = make_unique<TextureRenderer>(tex, step * i);
    d->frames.emplace_back(f.release());
  }
  auto f = make_unique<TextureRenderer>(tex, offset);
  d->frames.emplace_back(f.release());

  return move(*this);
}
