#include "../2048/View/Animation.h"

#include <gtest/gtest.h>

using namespace testing;

#include "TimerMock.h"
#include "RenderableMock.h"

using namespace std;

struct AnimationTest : public ::testing::Test {
  void SetUp() final {
    old_timer = Clip::timer();
    Clip::setTimer(&timer);

    renderable.reset(new RenderableMock());

    clip = Clip(dynamic_pointer_cast<IRenderable>(renderable));
  }

  void TearDown() final { Clip::setTimer(old_timer); }

  Timer *old_timer;
  TimerMock timer;
  shared_ptr<RenderableMock> renderable;
  Clip clip;
  Animation2 sut;
};

TEST_F(AnimationTest, given_render_1_clip_anim_when_not_exceed_duration_should_render_the_clip) {
  EXPECT_CALL(*renderable, draw(_, 0.8f)).Times(1);

  Animation2 sut = { clip.fadeby(-0.6f).duration(3) };

  timer.tick(1);
  sut.draw({});
}
