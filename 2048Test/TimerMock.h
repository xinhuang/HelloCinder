#pragma once

#include "../2048/View/Timer.h"

class TimerMock : public Timer {
  double elapsed_ = 0.0;

public:
  void tick(double elapsed) { elapsed_ += elapsed; }
  double elapsed() const final { return elapsed_; }
  void reset() final { elapsed_ = 0.0; }
};
