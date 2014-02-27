#pragma once

#include <string>
#include <stdexcept>

#include <cinder/Surface.h>

namespace networkx {

class Graph {
public:
  Graph();
  ~Graph();

  void addEdge(const std::string& from, const std::string& to) {}
  ci::Surface render() const { throw std::exception(); }
};

}
