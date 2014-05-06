#include "../2048/View/Control.h"

#include <gtest/gtest.h>

using namespace std;

using namespace ci;

struct ControlTest : public ::testing::Test {
  void SetUp() final {
    sut = make_shared<Control>();
    sut->setLocation({ 10.f, 10.f });
    sut->setSize({ 5.f, 5.f });
    child = sut->create<Control>();
  }

  void TearDown() final {}

  ControlRef sut;
  ControlRef child;
};

TEST_F(ControlTest, given_anchor_right_when_expand_width_should_move_left) {
  sut->setAnchor(Anchor::RIGHT);

  sut->setSize({ 6.f, 5.f });

  ASSERT_EQ(Vec2f(9.f, 10.f), sut->location());
}

TEST_F(ControlTest, given_anchor_bottom_t_when_expand_height_should_move_up) {
  sut->setAnchor(Anchor::BOTTOM);

  sut->setSize({ 5.f, 6.f });

  ASSERT_EQ(Vec2f(10.f, 9.f), sut->location());
}

TEST_F(ControlTest, given_child_control_anchor_left_when_expand_parent) {
  const float CHILD_X = 2.f;
  child->setSize({ 1.f, 2.f });
  child->setLocation({ CHILD_X, 3.f });
  child->setAnchor(Anchor::LEFT);

  sut->setSize({ 10.f, 10.f });

  ASSERT_EQ(CHILD_X, child->location().x);
}

TEST_F(ControlTest, given_child_control_anchor_right_when_expand_parent) {
  const float CHILD_X = 2.f;
  child->setSize({ 1.f, 2.f });
  child->setLocation({ CHILD_X, 3.f });
  child->setAnchor(Anchor::RIGHT);
  auto expect = sut->size().x - CHILD_X;

  sut->setSize({ 10.f, 10.f });

  ASSERT_EQ(expect, sut->size().x - child->location().x);
}

TEST_F(ControlTest, given_child_control_anchor_left_right_when_expand_parent) {
  const float CHILD_X = 2.f;
  child->setSize(sut->size());
  child->setLocation({});
  child->setAnchor(Anchor::LEFT_RIGHT);

  sut->setSize({ 10.f, 10.f });

  ASSERT_EQ(Vec2f(), child->location());
  ASSERT_EQ(Vec2f(10.f, 5.f), child->size());
}
