#pragma once

#include <deque>
#include <sstream>
#include <chrono>

class Universe;

class Sysinfo {
  size_t ngen_ = 0;
  size_t size_ = 0;
  size_t startSize_ = 0;
  const size_t MAX_GEN_COST = 10;
  std::chrono::steady_clock::time_point begin_;
  std::chrono::steady_clock::time_point systemStart_;
  mutable std::deque<std::chrono::milliseconds> costs_;

public:
  void onPreGen(const Universe& u) {
    begin_ = std::chrono::steady_clock::now();
  }

  void onPostGen(const Universe& u) {
    ++ngen_;
    size_ = u.size();
    auto now = std::chrono::steady_clock::now();
    costs_.push_front(
      std::chrono::duration_cast<std::chrono::milliseconds>(now - begin_));
  }

  std::chrono::seconds elapsed() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - systemStart_);
  }

  void init(const Universe& u) {
    ngen_ = 0;
    startSize_ = u.size();
    size_ = startSize_;
    systemStart_ = std::chrono::steady_clock::now();
  }

  std::string msg() const {
    std::ostringstream oss;
    oss << "#GEN \t" << ngen_ << std::endl << "SIZE \t" << size_ << " / " << startSize_
        << std::endl;
    if (costs_.size() == 0) {
      oss << "GPS \tINFINITE";
      return oss.str();
    }
    if (costs_.size() > MAX_GEN_COST)
      costs_.resize(MAX_GEN_COST);
    long long total{ 0 };
    for (const auto &ms : costs_)
      total += ms.count();
    auto average = total / costs_.size();
    if (average == 0) {
      oss << "GPS \tINFINITE";
      return oss.str();
    }
    auto gps = 1000 / average;
    oss << "GPS \t" << gps;
    return oss.str();
  }
};
