#include "stdafx.h"

#include <Rule.h>

using namespace std;

struct RuleTests : public ::testing::Test {
  Rule sut;
};

TEST_F(RuleTests, GIVEN_a_lonely_cell_WHEN_change_to_next_gen_SHOULD_die) {
  Cell cell({ 0, 0 }, CellState::ALIVE);

  Cell expect = sut.nextGeneration(cell, vector<Cell>{});

  ASSERT_TRUE(expect.isDead());
}

TEST_F(
    RuleTests,
    GIVEN_a_cell_with_2_alive_neighbor_WHEN_change_to_next_gen_SHOULD_survive) {
  Cell cell({ 0, 0 }, CellState::ALIVE);

  Cell expect =
      sut.nextGeneration(cell, vector<Cell>{ { { 1, 0 }, CellState::ALIVE },
                                             { { 0, 1 }, CellState::ALIVE } });

  ASSERT_FALSE(expect.isDead());
}
