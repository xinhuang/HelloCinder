#include "Environment.h"

using namespace ci;

using namespace std;

struct Environment::Data {
  Vec2f cellSize = { 32.f, 32.f };
};

Environment::Environment() : d(make_unique<Data>()) {}
Environment::~Environment() {}

const ci::Vec2f &Environment::cellSize() const { return d->cellSize; }
void Environment::setCellSize(const ci::Vec2f &size) { d->cellSize = size; }

ci::Vec2f Environment::distance(const ci::Vec2i& from, const ci::Vec2i& to) const {
  return Vec2f{ from - to } *cellSize();
}
