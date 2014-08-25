#pragma once

#include "Control.h"

#include <memory>

namespace ui {

class Button : public Control {
  struct Data;
  std::unique_ptr<Data> d;

public:
  Button();
  ~Button() override;
};

}
