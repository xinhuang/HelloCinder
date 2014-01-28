#pragma once

#include <memory>

namespace hcui {

struct Entity {
  virtual ~Entity() {}
  virtual void do_events() = 0;
};

enum class Anchor {
  Fill,
};

using EntityPtr = std::shared_ptr<Entity>;
}
