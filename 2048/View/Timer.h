#pragma once

#include <chrono>

class Timer {
  using time_point = std::chrono::steady_clock::time_point;

  time_point last_;

  static time_point now();

public:
  Timer() { last_ = now(); }
  virtual void reset() { last_ = now(); }
  virtual double elapsed() const;
};
