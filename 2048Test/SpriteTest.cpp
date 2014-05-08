#include "../2048/View/Sprite.h"

#include "MockSlice.h"
#include "MockGraphics.h"

#include <gtest/gtest.h>

using namespace testing;

using namespace std;

struct SpriteTest : public ::testing::Test {
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

  sut = { { 0, anim0 }, { 1, anim1 } };

  auto back = gfx();
  EXPECT_CALL(*graphics, frameInterval()).WillRepeatedly(Return(2.f));
  sut.draw({});
}
