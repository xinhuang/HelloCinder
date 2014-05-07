#include "../2048/View/VerticalLabel.h"

#include <gtest/gtest.h>

using namespace testing;

using namespace std;

TEST(VerticalLabelTest, given_set_label_height_should_text_in_correct_layout) {
  auto sut = make_shared<VerticalLabel>();
  sut->setSize({ 10.f, 10.f });

  sut->setLabelHeight(3.f);

  ASSERT_FLOAT_EQ(7.f, sut->textSize().y);
  ASSERT_FLOAT_EQ(3.f, sut->textLocation().y);
}
