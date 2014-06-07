#pragma once

#include "Control.h"

#include <memory>

class Button : public Control {
  struct Data;
  std::unique_ptr<Data> d;

public:

};