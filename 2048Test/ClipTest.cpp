#include "../2048/View/Animation.h"
#include "TimerMock.h"
#include "SliceMock.h"

using namespace ci;

#include <gtest/gtest.h>

using namespace ::testing;

#include <memory>

using namespace std;

struct ClipTest : public ::testing::Test {
  void SetUp() final {
    renderable.reset(new SliceMock());

    sut = Clip(dynamic_pointer_cast<Slice>(renderable));
  }

  shared_ptr<SliceMock> renderable;
  Clip sut;
};

TEST_F(ClipTest, given_move_when_tick_once_should_calculate_currect_location) {
  EXPECT_CALL(*renderable, draw(Rectf(1, 1, 2, 2), 1.f)).Times(1);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  sut.draw(0.1f, Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest, given_move_when_reversed_should_calculate_currect_location) {
  InSequence s;
  EXPECT_CALL(*renderable, draw(Rectf(3, 3, 4, 4), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 3, 3), 1.f)).Times(1);

  sut.moveby(Vec2f(3, 3)).duration(0.3f).reverse();

  sut.draw(0.0f, Rectf(0, 0, 1, 1));
  sut.draw(0.1f, Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest, given_move_when_tick_twice_should_calculate_currect_location) {
  EXPECT_CALL(*renderable, draw(Rectf(2.5, 2.5, 3.5, 3.5), 1.f)).Times(1);

  auto offset = Vec2f(3, 3);
  sut.moveby(offset).duration(0.3f);

  sut.draw(0.25f, Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest, given_move_when_tick_longer_than_duration_should_not_draw) {
  EXPECT_CALL(*renderable, draw(_, _)).Times(0);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  sut.draw(0.5f, Rectf());
}

TEST_F(ClipTest, given_move_when_render_at_every_0_1_seconds) {
  EXPECT_CALL(*renderable, draw(Rectf(1, 1, 2, 2), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 3, 3), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(3, 3, 4, 4), 1.f)).Times(1);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  sut.draw(0.1f, Rectf(0, 0, 1, 1));
  sut.draw(0.2f, Rectf(0, 0, 1, 1));
  sut.draw(0.3f, Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest,
       given_animation_when_fade_should_return_draw_with_correct_alpha) {
  EXPECT_CALL(*renderable, draw(_, 0.8f)).Times(1);

  sut.fadeby(-0.6f).duration(0.3f);

  sut.draw(0.1f, {});
}

TEST_F(ClipTest, given_fading_clip_when_reverse) {
  InSequence s;
  EXPECT_CALL(*renderable, draw(_, FloatNear(0.4f, 0.0001f))).Times(1);
  EXPECT_CALL(*renderable, draw(_, FloatNear(0.6f, 0.0001f))).Times(1);

  sut.fadeby(-0.6f).duration(0.3f).reverse();

  sut.draw(0.0f, {});
  sut.draw(0.1f, {});
}

TEST_F(ClipTest, given_scaling_clip_when_draw) {
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 4, 4), 1.f)).Times(1);

  sut.scale(0.f, 6.f).duration(0.3f);

  sut.draw(0.1f, Rectf(2, 2, 3, 3));
}

TEST_F(ClipTest, given_clip_when_reverse_scaling) {
  InSequence s;
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 8, 8), _)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 6, 6), _)).Times(1);

  sut.scale(0.f, 6.f).duration(0.3f).reverse();

  sut.draw(0.0f, Rectf(2, 2, 3, 3));
  sut.draw(0.1f, Rectf(2, 2, 3, 3));
}

TEST_F(ClipTest, given_move_fade_scale_should_draw_correctly) {
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 4, 4), 0.8f)).Times(1);

  sut.scale(0.f, 6.f).fadeby(-0.6f).duration(0.3f);

  sut.draw(0.1f, Rectf(2, 2, 3, 3));
}

TEST_F(ClipTest, given_empty_clip_when_draw_should_not_crash) {
  sut = Clip().duration(3.f);
  sut.draw(2.f, {});
}
