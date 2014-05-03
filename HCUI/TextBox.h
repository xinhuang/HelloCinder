#pragma once

#include "Common.h"
#include <memory>
#include <string>

namespace hcui {

class TextBox : public Entity {
  struct Impl;
  std::unique_ptr<Impl> m;

public:
  TextBox(int x, int y);

  void do_events() final;

  void set_anchor(Anchor v) {}
  void set_Text(std::string text);
};
}
