#include "NetworkXApp.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

#include <random>

using namespace std;

#include <boost/graph/adjacency_list.hpp>

using namespace boost;

#include "GraphWidget.h"

struct NetworkXApp::Data {
  GraphWidget gwidget_;
  Texture tex_;
  bool dragging_ = false;
  Vec2f offset_;
  Vec2f mouse_down_offset_;
};

NetworkXApp::NetworkXApp() : d(make_unique<Data>()) {}

NetworkXApp::~NetworkXApp() {}

void NetworkXApp::setup() {
  setFrameRate(60);

  vector<VertexDescriptor> vertices;

  auto engine = mt19937{ random_device()() };
  auto rand = [=](int range) mutable {
    auto idist = uniform_int_distribution<int>(0, range);
    return idist(engine);
  };

  UndirectedGraph graph;

  vertices.push_back(add_vertex(graph));
  for (int i = 0; i < 5; ++i) {
    auto v = add_vertex(graph);
    auto n = rand(4);
    for (int j = 0; j < n; ++j) {
      auto k = rand(vertices.size() - 1);
      add_edge(v, vertices[k], graph);
    }
    vertices.push_back(v);
  }

  d->gwidget_ = GraphWidget(graph, getWindowWidth(), getWindowHeight());
}

void NetworkXApp::update() {
  d->gwidget_.update();
}

void NetworkXApp::draw() {
  gl::setMatricesWindow(getWindowSize());
  gl::enableAlphaBlending();
  gl::clear(Color(0, 0, 0));

  gl::draw(d->gwidget_.render(d->offset_));
}

void NetworkXApp::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_ESCAPE:
    quit();
    break;
  }
}

void NetworkXApp::resize() {
  d->gwidget_.setSize(getWindowSize());
}

void NetworkXApp::mouseUp(MouseEvent e) {
  if (e.isLeft())
    d->dragging_ = false;
}

void NetworkXApp::mouseDrag(MouseEvent e) {
  if (d->dragging_) {
    d->offset_ = e.getPos() - d->mouse_down_offset_;
  }
}

void NetworkXApp::mouseDown(MouseEvent e) {
  if (e.isLeft()) {
    if (!d->dragging_) {
      d->dragging_ = true;
      d->mouse_down_offset_ = e.getPos() - d->offset_;
    }
  }
}

CINDER_APP_BASIC(NetworkXApp, RendererGl)
