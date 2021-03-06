#pragma once

#include "Universe.h"

#include <chrono>

#include <cinder/app/App.h>

class Sysinfo {
  const size_t MAX_GEN_COST = 10;
  bool fixedFps_ = false;
  size_t ngen_ = 0;
  size_t size_ = 0;
  std::chrono::steady_clock::time_point timestamp_;
  size_t count_ = 0;
  size_t fps_ = 0;
  std::string strategy_;
  std::string error_;

public:
  void onPreGen(const IUniverse &u) {}

  void onPostGen(const IUniverse &u) {
    ++ngen_;
    ++count_;
    size_ = u.size();
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - timestamp_);
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

  void init(const IUniverse &u) { init(u, ""); }

  void init(const IUniverse &u, const std::string &errmsg) {
    ngen_ = 0;
    count_ = 0;
    fps_ = 0;
    size_ = u.size();
    timestamp_ = std::chrono::steady_clock::now();
    strategy_ = u.name();
    error_ = errmsg;
  }

  std::string msg(const ci::app::App &app) const {
    char buf[256];
    _snprintf_s(buf, sizeof(buf) - 1,
                "<G>: Toggle information\n"
				"<Enter>: Re-generate universe\n"
				"<Space>: Pause\n"
				"<Left>/<Right>: Switch algorithm\n"
				"<UP>/<DOWN>: Set FPS\n"
				"\n"
				"GEN \t#%d\n"
				"SIZE\t%d\n"
				"FPS \t%f (%s)\n"
				"%s\n",
                ngen_, size_, app.getAverageFps(), fixedFps_ ? "Fixed" : "Unlimited", 
				strategy_.c_str());

    return error_.empty() ? buf : buf + ("\n" + error_);
  }

  void fixFps() { fixedFps_ = true; }
  void unfixFps() { fixedFps_ = false; }

  void setError(const std::string &reason) { error_ = "Error: " + reason; }
};
