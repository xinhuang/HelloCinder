#pragma once

#include <../2048/View/Graphics.h>
#include <gmock/gmock.h>

struct MockGraphics : public Graphics, public std::enable_shared_from_this<Graphics> {
  MOCK_CONST_METHOD0(frameInterval, float());

  void install() {
    back_ = Graphics::instance();
    Graphics::instance() = shared_from_this();
  }

  void uninstall() {
    Graphics::instance() = back_;
    back_ = nullptr;
  }

  void setFrameInterval(float seconds) {
    EXPECT_CALL(*this, frameInterval()).WillRepeatedly(::testing::Return(seconds));
  }

private:
  std::shared_ptr<Graphics> back_;
};
