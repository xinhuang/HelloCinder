#pragma once

#include <../2048/View/Gfx.h>
#include <gmock/gmock.h>

struct MockGraphics : public animation::Gfx, public std::enable_shared_from_this<animation::Gfx> {
  MOCK_CONST_METHOD0(frameInterval, float());

  void install() {
    back_ = Gfx::instance();
    Gfx::instance() = shared_from_this();
  }

  void uninstall() {
    Gfx::instance() = back_;
    back_ = nullptr;
  }

  void setFrameInterval(float seconds) {
    EXPECT_CALL(*this, frameInterval()).WillRepeatedly(::testing::Return(seconds));
  }

private:
  std::shared_ptr<Gfx> back_;
};
