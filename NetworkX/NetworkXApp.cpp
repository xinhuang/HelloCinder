#include "NetworkXApp.h"

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

using namespace std;

#include "Graph.h"

using namespace networkx;

struct NetworkXApp::Data {
  unique_ptr<Graph> graph_;
  Texture tex_;
};

NetworkXApp::NetworkXApp() : d(make_unique<Data>()) {}

NetworkXApp::~NetworkXApp() {}

void NetworkXApp::setup() {
  d->graph_ = make_unique<Graph>();
  d->graph_->addEdge("A", "B");
}

void NetworkXApp::update() {
  d->tex_ = Texture(d->graph_->render());
}

void NetworkXApp::draw() {
  gl::setMatricesWindow(getWindowSize());
  gl::enableAlphaBlending();
  gl::clear(Color(0, 0, 0));

  if (d->tex_)
    gl::draw(d->tex_);
}

CINDER_APP_BASIC(NetworkXApp, RendererGl)
