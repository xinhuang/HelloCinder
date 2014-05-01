#include "../2048/View/Animation.h"
#include "TimerMock.h"
#include "RenderableMock.h"

using namespace ci;

#include <gtest/gtest.h>

using namespace ::testing;

#include <memory>

using namespace std;

struct AnimationTest : public ::testing::Test {
  void SetUp() final {
    old_timer = Animation2::timer();
    Animation2::setTimer(&timer);

    renderable.reset(new RenderableMock());

    sut = Animation2(dynamic_pointer_cast<IRenderable>(renderable));
  }

  void TearDown() final { Animation2::setTimer(old_timer); }

  Timer *old_timer;
  TimerMock timer;
  shared_ptr<RenderableMock> renderable;
  Animation2 sut;
};

TEST_F(AnimationTest,
       given_move_when_tick_once_should_calculate_currect_location) {
  EXPECT_CALL(*renderable, draw(Rectf(1, 1, 2, 2), 1.f)).Times(1);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  timer.tick(0.1);
  sut.draw(Rectf(0, 0, 1, 1));
}

TEST_F(AnimationTest,
       given_move_when_tick_twice_should_calculate_currect_location) {
  EXPECT_CALL(*renderable, draw(Rectf(2.5, 2.5, 3.5, 3.5), 1.f)).Times(1);

  auto offset = Vec2f(3, 3);
  sut.moveby(offset).duration(0.3f);

  timer.tick(0.25);
  sut.draw(Rectf(0, 0, 1, 1));
}

TEST_F(
    AnimationTest,
    given_move_when_tick_longer_than_duration_should_not_draw) {
  EXPECT_CALL(*renderable, draw(_, _)).Times(0);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  timer.tick(0.5);
  sut.draw(Rectf());
}

TEST_F(AnimationTest, given_move_when_render_at_every_0_1_seconds) {
  EXPECT_CALL(*renderable, draw(Rectf(1, 1, 2, 2), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 3, 3), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(3, 3, 4, 4), 1.f)).Times(1);
  
  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  timer.tick(0.1);
  ASSERT_EQ(Vec2f(1, 1), sut.offset());
  sut.draw(Rectf(0, 0, 1, 1));
  timer.reset();

  timer.tick(0.1);
  ASSERT_EQ(Vec2f(2, 2), sut.offset());
  sut.draw(Rectf(0, 0, 1, 1));
  timer.reset();

  timer.tick(0.1);
  ASSERT_EQ(Vec2f(3, 3), sut.offset());
  sut.draw(Rectf(0, 0, 1, 1));
  timer.reset();
}

TEST_F(AnimationTest, given_animation_when_fade_should_return_correct_alpha) {
  EXPECT_CALL(*renderable, draw(_, 0.8f)).Times(1);

  sut.fadeby(-0.6f).duration(0.3f);

  timer.tick(0.1);
  sut.draw({});
}
