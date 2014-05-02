#include "Timer.h"

#include <chrono>

using namespace std;

Timer::time_point Timer::now() {
  return std::chrono::steady_clock::now();
}

double Timer::elapsed() const {
  auto duration = now() - last_;
  return chrono::duration_cast<chrono::milliseconds>(duration).count() * 0.001;
}
