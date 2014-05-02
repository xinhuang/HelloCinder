#include "Animation.h"

#include "Timer.h"

using namespace ci;

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

struct Animation::Data {
  static unique_ptr<Timer> timer;

  bool cyclic = false;
  float elapsed = 0;
  vector<Clip> clips;
};

unique_ptr<Timer> Animation::Data::timer = make_unique<Timer>();

Animation::Animation() : d(make_unique<Data>()) {}

Animation::~Animation() {}

Animation::Animation(const Animation& anim) : Animation() {
  *this = anim;
}

Animation::Animation(const std::initializer_list<Clip> &clips)
    : Animation() {
  d->clips = clips;
}

Animation& Animation::operator=(const Animation& anim) {
  *d = *(anim.d);
  return *this;
}

void Animation::rewind() {
  for (auto& clip : d->clips)
    clip.rewind();
}

Animation& Animation::cyclic(bool value) {
  d->cyclic = value;
  return *this;
}

void Animation::draw(const ci::Rectf &rect) {
  float frame_interval = (float)Data::timer->elapsed();
  d->elapsed += frame_interval;

  float anim_duration = duration();
  if (d->cyclic && d->elapsed > anim_duration)
    d->elapsed = fmod(d->elapsed, anim_duration);
  if (anim_duration < d->elapsed) {
    assert(!d->cyclic);
    return;
  }

  float elapsed = d->elapsed;
  for (auto &clip : d->clips) {
    if (elapsed <= clip.duration()) {
      clip.update(frame_interval);
      clip.draw(elapsed, rect);
      return;
    } else {
      elapsed -= clip.duration();
    }
  }
}

float Animation::duration() const {
  return accumulate(d->clips.begin(), d->clips.end(), 0.f,
                    [&](float v, const Clip &c) { return v + c.duration(); });
}

void Animation::setTimer(Timer *timer) {
  Data::timer.release();
  Data::timer.reset(timer);
}

Timer *Animation::timer() { return Data::timer.get(); }

Animation operator+(Animation lhs, const Animation &rhs) {
  lhs.d->clips.insert(lhs.d->clips.end(), rhs.d->clips.begin(),
                      rhs.d->clips.end());
  return lhs;
}
