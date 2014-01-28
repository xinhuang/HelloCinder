#pragma once

#include <deque>
#include <sstream>
#include <chrono>

class Universe;

class Sysinfo {
  const size_t MAX_GEN_COST = 10;
  std::chrono::steady_clock::time_point begin_;
  mutable std::deque<std::chrono::milliseconds> costs_;

public:
  void onPreGen(const Universe& u) {
    begin_ = std::chrono::steady_clock::now();
  }

  void onPostGen(const Universe& u) {
    auto now = std::chrono::steady_clock::now();
    costs_.push_front(
      std::chrono::duration_cast<std::chrono::milliseconds>(now - begin_));
  }

  std::string msg() const {
    if (costs_.size() > MAX_GEN_COST)
      costs_.resize(MAX_GEN_COST);
    long long total{ 0 };
    for (const auto &ms : costs_)
      total += ms.count();
    auto average = total / costs_.size();
    if (average == 0)
      return "GPS: INFINITE";
    auto gps = 1000 / average;
    std::ostringstream oss;
    oss << "GPS: " << gps;
    return oss.str();
  }
};
