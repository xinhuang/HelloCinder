#include "../2048/View/Animation.h"
#include "TimerMock.h"
#include "RenderableMock.h"

using namespace ci;

#include <gtest/gtest.h>

using namespace ::testing;

#include <memory>

using namespace std;

struct ClipTest : public ::testing::Test {
  void SetUp() final {
    renderable.reset(new RenderableMock());

    sut = Clip(dynamic_pointer_cast<IRenderable>(renderable));
  }

  shared_ptr<RenderableMock> renderable;
  Clip sut;
};

TEST_F(ClipTest, given_move_when_tick_once_should_calculate_currect_location) {
  EXPECT_CALL(*renderable, draw(Rectf(1, 1, 2, 2), 1.f)).Times(1);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  sut.update(0.1f);
  sut.draw(Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest, given_move_when_tick_twice_should_calculate_currect_location) {
  EXPECT_CALL(*renderable, draw(Rectf(2.5, 2.5, 3.5, 3.5), 1.f)).Times(1);

  auto offset = Vec2f(3, 3);
  sut.moveby(offset).duration(0.3f);

  sut.update(0.25f);
  sut.draw(Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest, given_move_when_tick_longer_than_duration_should_not_draw) {
  EXPECT_CALL(*renderable, draw(_, _)).Times(0);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  sut.update(0.5);
  sut.draw(Rectf());
}

TEST_F(ClipTest, given_move_when_render_at_every_0_1_seconds) {
  EXPECT_CALL(*renderable, draw(Rectf(1, 1, 2, 2), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 3, 3), 1.f)).Times(1);
  EXPECT_CALL(*renderable, draw(Rectf(3, 3, 4, 4), 1.f)).Times(1);

  sut.moveby(Vec2f(3, 3)).duration(0.3f);

  sut.update(0.1f);
  ASSERT_EQ(Vec2f(1, 1), sut.offset());
  sut.draw(Rectf(0, 0, 1, 1));

  sut.update(0.1f);
  ASSERT_EQ(Vec2f(2, 2), sut.offset());
  sut.draw(Rectf(0, 0, 1, 1));

  sut.update(0.1f);
  ASSERT_EQ(Vec2f(3, 3), sut.offset());
  sut.draw(Rectf(0, 0, 1, 1));
}

TEST_F(ClipTest,
       given_animation_when_fade_should_return_draw_with_correct_alpha) {
  EXPECT_CALL(*renderable, draw(_, 0.8f)).Times(1);

  sut.fadeby(-0.6f).duration(0.3f);

  sut.update(0.1f);
  sut.draw({});
}

TEST_F(ClipTest,
       given_animation_when_zoom_should_return_draw_with_correct_size) {
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 5, 5), 1.f)).Times(1);

  sut.scaleby(6.f).duration(0.3f);

  sut.update(0.1f);
  sut.draw(Rectf(2, 2, 3, 3));
}

TEST_F(ClipTest, given_move_fade_scale_should_draw_correctly) {
  EXPECT_CALL(*renderable, draw(Rectf(2, 2, 5, 5), 0.8f)).Times(1);

  sut.scaleby(6.f).fadeby(-0.6f).duration(0.3f);

  sut.update(0.1f);
  sut.draw(Rectf(2, 2, 3, 3));
}
