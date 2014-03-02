#include "GraphWidget.h"

#include <cinder/gl/fbo.h>

using namespace ci;
using namespace boost;

#include <utility>
#include <cassert>
#include <random>
#include <unordered_map>

using namespace std;

#include "Random.h"

struct Vertex {
  Vertex() {}
  Vertex(const ci::Vec2f &pos) : pos_(pos) {}

  Vertex(const Vertex &v) : pos_(v.pos_), disp_(v.disp_) {}
  Vertex(const Vertex &&v) NOEXCEPT : pos_(std::move(v.pos_)),
                                      disp_(std::move(v.disp_)) {}

  ci::Vec2f pos_;
  ci::Vec2f disp_;
};

void spring(const UndirectedGraph &g,
            std::unordered_map<VertexDescriptor, Vertex> &vertices, int width,
            int height, int iteration) {
  if (iteration > 100)
    return;

  const float C1 = 2.;
  const float C2 = 50.;
  const float C3 = 1;
  const float C4 = 0.1f;

  for (auto &v1 : vertices) {
    v1.second.disp_ = {};
    for (const auto &v2 : vertices) {
      if (v1.first == v2.first)
        continue;
      auto distance = v2.second.pos_ - v1.second.pos_;
      auto length = distance.length();
      if (length - 0.f < 0.0000001f)
        length = 1.f;
      auto r = edge(v1.first, v2.first, g);
      if (r.second) {
        v1.second.disp_ += C1 * log10(length / C2) * distance;
      } else {
        v1.second.disp_ -= C3 / sqrt(length) * distance;
      }
    }
  }

  for (auto &v : vertices) {
    auto old = v.second.pos_;
    v.second.pos_ += C4 * v.second.disp_.normalized() * v.second.disp_.length();
    if (v.second.pos_.x < 0 || v.second.pos_.x > width)
      v.second.pos_.x = old.x;
    if (v.second.pos_.y < 0 || v.second.pos_.y > height)
      v.second.pos_.y = old.y;
  }
}

void
fruchtermanReingold(const UndirectedGraph &g,
                    std::unordered_map<VertexDescriptor, Vertex> &vertices, int width, int height, int iteration) {
  if (iteration > 100)
    return;

  float k = width * height / static_cast<float>(vertices.size());
  auto fr = [=](float x) { return k / x; };
  auto sqrt_k = sqrt(k);
  auto fa = [=](float x) { return x * x / sqrt_k; };
  
  for (auto &v1 : vertices) {
    v1.second.disp_ = {};
    for (const auto &v2 : vertices) {
      if (v1.first == v2.first)
        continue;
      auto distance = v2.second.pos_ - v1.second.pos_;
      auto length = distance.length();
      Vec2f normalized{
        distance.x / length, distance.y / length
      }; // distance.normalized() will calculate length again.
      v1.second.disp_ -= normalized * fr(length);
    }
  }

  for (auto e = edges(g).first; e != edges(g).second; ++e) {
    auto ud = source(*e, g);
    auto vd = target(*e, g);
    assert(vertices.find(ud) != vertices.end());
    assert(vertices.find(vd) != vertices.end());
    auto& u = vertices[ud];
    auto& v = vertices[vd];
    auto distance = v.pos_ - u.pos_;
    u.disp_ += distance.normalized() / fa(distance.length());
    v.disp_ -= distance.normalized() / fa(distance.length());
  }

  auto temp = 10.f / iteration;
  for (auto &v : vertices) {
    v.second.pos_ += v.second.disp_.normalized() * min(v.second.disp_.length(), temp);
    v.second.pos_.x = min(static_cast<float>(width), max(0.f, v.second.pos_.x));
    v.second.pos_.y = min(static_cast<float>(height), max(0.f, v.second.pos_.y));
  }
}

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
  if (size == d->fbo_.getSize())
    return;
  d->fbo_ = gl::Fbo(size.x, size.y);
  d->fbo_updated_ = false;
  d->i_ = 0;
}

UndirectedGraph &GraphWidget::graph() { return d->g_; }

void GraphWidget::update() {
  ++d->i_;
  d->fbo_updated_ = false;

  //spring(d->g_, d->vertices_, width(), height(), d->i_);
  fruchtermanReingold(d->g_, d->vertices_, width(), height(), d->i_);
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
    gl::drawSolidCircle(v.second.pos_ + offset, VERTEX_RADIUS);
  }

  d->fbo_.unbindFramebuffer();

  return d->fbo_.getTexture();
}
