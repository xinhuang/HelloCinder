#include "GraphWidget.h"

#include <cinder/gl/fbo.h>

using namespace ci;
using namespace boost;

#include <utility>
#include <unordered_map>
#include <cassert>
#include <random>

using namespace std;

#include "Random.h"

struct Vertex {
  Vertex() {}
  Vertex(const Vec2f &pos) : pos_(pos) {}

  Vertex(const Vertex &v) : pos_(v.pos_), disp_(v.disp_) {}
  Vertex(const Vertex &&v) NOEXCEPT : pos_(std::move(v.pos_)),
                                      disp_(std::move(v.disp_)) {}

  Vec2f pos_;
  Vec2f disp_;
};

struct GraphWidget::Data {
  Data() {}
  Data(const UndirectedGraph &g, int width, int height) : Data() {
    g_ = g;
    fbo_ = gl::Fbo(width, height);
  }

  bool fbo_updated_ = false;
  int i_ = 0;
  std::unordered_map<VertexDescriptor, Vertex> vertices_;
  UndirectedGraph g_;
  gl::Fbo fbo_;
};

GraphWidget::GraphWidget() : d(make_unique<Data>()) {}

GraphWidget::GraphWidget(const UndirectedGraph &g, int width, int height)
    : d(make_unique<Data>(g, width, height)) {
  VertexIterator v, v_end;
  for (tie(v, v_end) = vertices(g); v != v_end; ++v) {
    d->vertices_[*v] = { { Random::next<float>(600.f),
                           Random::next<float>(400.f) } };
  }
}

GraphWidget::~GraphWidget() {}

GraphWidget &GraphWidget::operator=(const GraphWidget &gw) {
  d = make_unique<Data>();
  d->fbo_updated_ = gw.d->fbo_updated_;
  d->i_ = gw.d->i_;
  d->vertices_ = gw.d->vertices_;
  d->g_ = gw.d->g_;
  d->fbo_ = gw.d->fbo_;

  return *this;
}

int GraphWidget::width() const { return d->fbo_.getWidth(); }

int GraphWidget::height() const { return d->fbo_.getHeight(); }

void GraphWidget::setSize(const Vec2i &size) {
  d->fbo_ = gl::Fbo(size.x, size.y);
  d->fbo_updated_ = false;
}

UndirectedGraph &GraphWidget::graph() { return d->g_; }

void GraphWidget::update() {
  // if (d->i_ > M)
  //  return;
  ++d->i_;
  d->fbo_updated_ = false;

  for (auto &v1 : d->vertices_) {
    v1.second.disp_ = {};
    for (const auto &v2 : d->vertices_) {
      if (v1.first == v2.first)
        continue;
      auto distance = v2.second.pos_ - v1.second.pos_;
      auto length = distance.length();
      if (length - 0.f < 0.0000001f)
        length = 1.f;
      auto r = edge(v1.first, v2.first, d->g_);
      if (r.second) {
        v1.second.disp_ += C1 * log10(length / C2) * distance;
      } else {
        // if (length < 30.f)
        v1.second.disp_ -= C3 / sqrt(length) * distance;
      }
    }
  }

  for (auto &v : d->vertices_) {
    auto old = v.second.pos_;
    v.second.pos_ += C4 * v.second.disp_;
    if (v.second.pos_.x < 0 || v.second.pos_.x > d->fbo_.getWidth())
      v.second.pos_.x = old.x;
    if (v.second.pos_.y < 0 || v.second.pos_.y > d->fbo_.getHeight())
      v.second.pos_.y = old.y;
  }
}

const gl::Texture &GraphWidget::render(const ci::Vec2i &offset) const {
  if (d->fbo_updated_)
    return d->fbo_.getTexture();
  d->fbo_updated_ = true;

  d->fbo_.bindFramebuffer();
  gl::clear();

  gl::color(Color(0.46f, 0.46f, 0.46f));
  for (auto ei = edges(d->g_); ei.first != ei.second; ++ei.first) {
    const auto &vi1 = source(*ei.first, d->g_);
    const auto &vi2 = target(*ei.first, d->g_);

    assert(d->vertices_.find(vi1) != d->vertices_.end());
    assert(d->vertices_.find(vi2) != d->vertices_.end());

    const auto &v1 = d->vertices_.find(vi1)->second;
    const auto &v2 = d->vertices_.find(vi2)->second;

    gl::drawLine(v1.pos_ + offset, v2.pos_ + offset);
  }

  gl::color(Color::white());
  for (const auto &v : d->vertices_) {
    gl::drawSolidCircle(v.second.pos_ + offset, 2.f);
  }

  d->fbo_.unbindFramebuffer();

  return d->fbo_.getTexture();
}
