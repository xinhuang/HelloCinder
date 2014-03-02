#pragma once

#include <boost/graph/adjacency_list.hpp>

using UndirectedGraph =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                          boost::no_property, boost::no_property>;

using EdgeIterator = boost::graph_traits<UndirectedGraph>::edge_iterator;
using InEdgeIterator = boost::graph_traits<UndirectedGraph>::in_edge_iterator;
using VertexIterator = boost::graph_traits<UndirectedGraph>::vertex_iterator;
using VertexDescriptor =
    boost::graph_traits<UndirectedGraph>::vertex_descriptor;
using AdjacencyIterator =
    boost::graph_traits<UndirectedGraph>::adjacency_iterator;

#include <cinder/gl/Texture.h>

#include <memory>

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif // _MSC_VER

class GraphWidget {

public:
  GraphWidget();
  GraphWidget(const UndirectedGraph &g, int width, int height);

  GraphWidget &operator=(const GraphWidget &g);

  ~GraphWidget();

  void update();
  const ci::gl::Texture &render(const ci::Vec2i &offset = {}) const;

  int width() const;
  int height() const;

  void setSize(const ci::Vec2i &size);

  UndirectedGraph &graph();

private:
  struct Data;
  std::unique_ptr<Data> d;

  const int M = 100;
  const float VERTEX_RADIUS = 5.f;
};
