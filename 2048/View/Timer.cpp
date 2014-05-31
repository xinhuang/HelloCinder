#include "Timer.h"

#include <chrono>

using namespace std;

Timer::time_point Timer::now() {
  return std::chrono::steady_clock::now();
}

double Timer::elapsed() const {
//#define FIXED_TIMER 0.01
#if !defined FIXED_TIMER
  auto duration = now() - last_;
  return chrono::duration_cast<chrono::milliseconds>(duration).count() * 0.001;
#else
  auto duration = last_ - last_;
  return chrono::duration_cast<chrono::milliseconds>(duration).count() + FIXED_TIMER;
#endif
}
