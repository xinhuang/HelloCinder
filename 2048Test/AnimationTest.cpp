#include "../2048/View/animation/Animation.h"

#include "MockSlice.h"
#include "MockGraphics.h"

#include <gtest/gtest.h>

using namespace testing;

using namespace std;

using namespace animation;

struct AnimationTest : public ::testing::Test {
  void SetUp() final {
    graphics = make_shared<MockGraphics>();
    graphics->install();
    renderables.emplace_back(new MockSlice());
    renderables.emplace_back(new MockSlice());

    clips.emplace_back(dynamic_pointer_cast<Slice>(renderables[0]));
    clips.emplace_back(dynamic_pointer_cast<Slice>(renderables[1]));
  }

  void TearDown() final { graphics->uninstall(); }

  shared_ptr<MockGraphics> graphics;
  vector<shared_ptr<MockSlice> > renderables;
  vector<Clip> clips;
  Animation sut;
};

TEST_F(
    AnimationTest,
    given_render_1_clip_anim_when_not_exceed_duration_should_render_the_clip) {
  EXPECT_CALL(*renderables[0], draw(_, 0.8f)).Times(1);
  sut = { clips[0].fadeby(-0.6f).duration(3) };
  graphics->setFrameInterval(1.f);

  sut.draw({});
}

TEST_F(AnimationTest, given_render_2_clips_anim_when_no_interleaving) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.4f, 0.001f))).Times(1);
    EXPECT_CALL(*renderables[1], draw(_, FloatNear(0.3f, 0.001f))).Times(1);
  }
  sut = { clips[0].fadeby(-0.6f).duration(3),
          clips[1].fadeby(-0.7f).duration(1) };

  graphics->setFrameInterval(3);
  sut.draw({});

  graphics->setFrameInterval(1.f);
  sut.draw({});
}

TEST_F(AnimationTest, given_render_2_clips_anim_when_there_is_interleaving) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.6f, 0.001f))).Times(1);
    EXPECT_CALL(*renderables[1], draw(_, FloatNear(0.3f, 0.001f))).Times(1);
  }
  sut = { clips[0].fadeby(-0.6f).duration(3),
          clips[1].fadeby(-0.7f).duration(1) };
  graphics->setFrameInterval(2);

  sut.draw({});
  sut.draw({});
}

TEST_F(AnimationTest,
       given_render_2_clips_anim_when_1st_clip_should_be_skipped) {
  EXPECT_CALL(*renderables[1], draw(_, FloatNear(0.3f, 0.001f))).Times(1);
  sut = { clips[0].fadeby(-0.6f).duration(3),
          clips[1].fadeby(-0.7f).duration(1) };
  graphics->setFrameInterval(4);

  sut.draw({});
}

TEST_F(AnimationTest, given_render_1_clip_anim_when_wrap_as_loop) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, _)).Times(2);
  }

  sut = { clips[0].duration(3) };

  sut.wrap(WrapMode::LOOP);

  graphics->setFrameInterval(1);
  sut.draw({});
  graphics->setFrameInterval(2);
  sut.draw({});
}

TEST_F(AnimationTest, given_render_2_clips_anim_when_wrap_as_loop) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.8f, 0.001f))).Times(1);
    EXPECT_CALL(*renderables[1], draw(_, FloatNear(0.3f, 0.001f))).Times(1);
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.6f, 0.001f))).Times(1);
  }

  sut = { clips[0].fadeby(-0.6f).duration(3),
          clips[1].fadeby(-0.7f).duration(1) };

  sut.wrap(WrapMode::LOOP);

  graphics->setFrameInterval(1);
  sut.draw({});

  graphics->setFrameInterval(3);
  sut.draw({});

  graphics->setFrameInterval(2);
  sut.draw({});
}

TEST_F(AnimationTest, given_render_1_clip_anim_when_wrap_as_clamp_forever) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.9f, 0.001f))).Times(1);
    EXPECT_CALL(*renderables[0], draw(_, FloatNear(0.8f, 0.001f))).Times(2);
  }

  sut = { clips[0].fadeby(-0.2f).duration(2) };

  sut.wrap(WrapMode::CLAMP_FOREVER);

  graphics->setFrameInterval(1);
  sut.draw({});
  sut.draw({});
  sut.draw({});
}
