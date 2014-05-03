#pragma once

#include "Control.h"

class UI {
  void addControl(const ControlRef& control);

public:
  void setup();
  void tearDown();

  template <typename T>
  std::shared_ptr<T> create() {
    auto v = std::make_shared<T>();
    addControl(std::dynamic_pointer_cast<ControlRef>(v));
    return v;
  }
};
