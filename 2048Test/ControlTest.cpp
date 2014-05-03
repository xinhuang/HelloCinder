#include "../2048/View/Control.h"

#include <gtest/gtest.h>

using namespace std;

using namespace ci;

struct AnchorTest : public ::testing::Test {
  void SetUp() final { sut = make_shared<Control>(); }

  void TearDown() final {}

  ControlRef sut;
};

TEST_F(AnchorTest, given_anchor_right_when_expand_width_should_move_left) {
  sut->setLocation({ 10.f, 10.f });
  sut->setSize({ 5.f, 5.f });
  sut->setAnchor(Anchor::RIGHT);

  sut->setSize({ 6.f, 5.f });

  ASSERT_EQ(Vec2f(9.f, 10.f), sut->location());
}

TEST_F(AnchorTest, given_anchor_bottom_t_when_expand_height_should_move_up) {
  sut->setLocation({ 10.f, 10.f });
  sut->setSize({ 5.f, 5.f });
  sut->setAnchor(Anchor::BOTTOM);

  sut->setSize({ 5.f, 6.f });

  ASSERT_EQ(Vec2f(10.f, 9.f), sut->location());
}