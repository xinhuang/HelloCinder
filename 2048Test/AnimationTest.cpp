#include "../2048/View/Animation.h"

#include "TimerMock.h"
#include "RenderableMock.h"

#include <gtest/gtest.h>

using namespace testing;

using namespace std;

struct AnimationTest : public ::testing::Test {
  void SetUp() final {
    old_timer = Clip::timer();
    Clip::setTimer(&timer);

    renderables.emplace_back(new RenderableMock());
    renderables.emplace_back(new RenderableMock());

    clips.emplace_back(dynamic_pointer_cast<IRenderable>(renderables[0]));
    clips.emplace_back(dynamic_pointer_cast<IRenderable>(renderables[1]));
  }

  void TearDown() final { Clip::setTimer(old_timer); }

  Timer *old_timer;
  TimerMock timer;
  vector<shared_ptr<RenderableMock>> renderables;
  vector<Clip> clips;
  Animation2 sut;
};

TEST_F(AnimationTest, given_render_1_clip_anim_when_not_exceed_duration_should_render_the_clip) {
  EXPECT_CALL(*renderables[0], draw(_, 0.8f)).Times(1);

  Animation2 sut = { clips[0].fadeby(-0.6f).duration(3) };

  timer.tick(1);
  sut.draw({});
}

TEST_F(AnimationTest, given_render_2_clips_anim_when_1st_just_finished_should_continue_with_2nd) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.4f, 0.001f))).Times(1);
    EXPECT_CALL(*renderables[1], draw(_, FloatNear(0.3f, 0.001f))).Times(1);
  }

  Animation2 sut = { clips[0].fadeby(-0.6f).duration(3), clips[1].fadeby(-0.7f).duration(1) };

  timer.tick(3);
  sut.draw({});
  timer.reset();
  timer.tick(1);
  sut.draw({});
}
