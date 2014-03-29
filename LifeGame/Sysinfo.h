#pragma once

#include <deque>
#include <sstream>
#include <chrono>

#include <cinder/app/App.h>

class Sysinfo {
  const size_t MAX_GEN_COST = 10;
  size_t ngen_ = 0;
  size_t size_ = 0;
  std::chrono::steady_clock::time_point timestamp_;
  size_t count_ = 0;
  size_t fps_ = 0;
  std::string strategy_;

public:
  void onPreGen(const IUniverse &u) {}

  void onPostGen(const IUniverse& u) {
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

  void init(const IUniverse& u) {
    ngen_ = 0;
    count_ = 0;
    fps_ = 0;
    size_ = u.size();
    timestamp_ = std::chrono::steady_clock::now();
    strategy_ = u.name();
  }

  std::string msg(const ci::app::App& app) const {
    std::ostringstream oss;
    oss << "#GEN \t" << ngen_ << std::endl << "SIZE \t" << size_ << std::endl
        << "FPS \t" << app.getAverageFps() << std::endl
        << "Strategy \t" << strategy_;
    return oss.str();
  }
};
