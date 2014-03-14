#pragma once

#include <deque>
#include <sstream>
#include <chrono>

class Universe;

class Sysinfo {
  const size_t MAX_GEN_COST = 10;
  size_t ngen_ = 0;
  size_t size_ = 0;
  std::chrono::steady_clock::time_point timestamp_;
  size_t count_ = 0;
  size_t fps_ = 0;

public:
  void onPreGen(const Universe &u) {}

  void onPostGen(const Universe& u) {
    ++ngen_;
    ++count_;
    size_ = u.size();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp_);
    if (elapsed > std::chrono::seconds(1)) {
      fps_ = count_;
      count_ = 0;
      timestamp_ = now;
    }
  }

  std::chrono::seconds elapsed() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - timestamp_);
  }

  void init(const Universe& u) {
    ngen_ = 0;
    count_ = 0;
    fps_ = 0;
    size_ = u.size();
    timestamp_ = std::chrono::steady_clock::now();
  }

  std::string msg() const {
    std::ostringstream oss;
    oss << "#GEN \t" << ngen_ << std::endl << "SIZE \t" << size_ << std::endl;
    oss << "FPS \t" << fps_;
    return oss.str();
  }
};
