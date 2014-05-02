#include "../2048/View/Sprite.h"

#include "TimerMock.h"
#include "RenderableMock.h"

#include <gtest/gtest.h>

using namespace testing;

using namespace std;

struct SpriteTest : public ::testing::Test {
  void SetUp() final {
    old_timer = Animation::timer();
    Animation::setTimer(&timer);

    renderables.emplace_back(new RenderableMock());
    renderables.emplace_back(new RenderableMock());

    clips.emplace_back(dynamic_pointer_cast<IRenderable>(renderables[0]));
    clips.emplace_back(dynamic_pointer_cast<IRenderable>(renderables[1]));
  }

  void TearDown() final { Animation::setTimer(old_timer); }

  Timer *old_timer;
  TimerMock timer;
  vector<shared_ptr<RenderableMock> > renderables;
  vector<Clip> clips;
  Sprite sut;
};

TEST_F(SpriteTest, when_has_2_layers_should_draw_bigger_layer_number_later) {
  {
    InSequence s;
    EXPECT_CALL(*renderables[0], draw(_, _)).Times(1);
    EXPECT_CALL(*renderables[1], draw(_, _)).Times(1);
  }

  Animation anim0 = { clips[0].fadeby(-3.f).duration(3.f) },
             anim1 = { clips[1].fadeby(-3.f).duration(3.f) };

  sut = { { 0, anim0 },
          { 1, anim1 } };

  timer.tick(2);
  sut.draw({});
}
