#include "stdafx.h"

#include <Universe.h>

using namespace std;

TEST(UniverseTests, the_big_bang) {
  Universe sut;

  ASSERT_TRUE(sut.isSilent());
}

TEST(UniverseTests, extinction) {
  Universe sut;

  sut.add({ 3, 3 }, CellState::ALIVE);

  ASSERT_TRUE(!sut.isSilent());
}

TEST(UniverseTests, survive_if_2_neighbors) {
  Universe sut;
  sut.add({0, 0 }, CellState::ALIVE);
  sut.add({ 0, 1 }, CellState::ALIVE);
  sut.add({ 0, 2 }, CellState::ALIVE);

  Universe now;
  sut.nextGeneration(now);

  auto cell = now[{ 0, 1 }];
  ASSERT_FALSE(cell.isDead());
}

TEST(UniverseTests, birth_if_3_neighbors) {
  Universe sut;
  sut.add({ 0, 0 }, CellState::ALIVE);
  sut.add({ 2, 0 }, CellState::ALIVE);
  sut.add({ 1, 1 }, CellState::ALIVE);

  Universe now;
  sut.nextGeneration(now);

  ASSERT_TRUE(!now.isSilent());
}

TEST(UniverseTests, square_is_a_stable_pattern) {
  Universe sut;
  sut.add({ 3, 3 }, CellState::ALIVE);
  sut.add({ 3, 4 }, CellState::ALIVE);
  sut.add({ 4, 3 }, CellState::ALIVE);
  sut.add({ 4, 4 }, CellState::ALIVE);

  Universe expect;
  sut.nextGeneration(expect);

  ASSERT_TRUE(!expect.isSilent());

  expect.add({ 3, 3 }, CellState::DEAD);
  expect.add({ 3, 4 }, CellState::DEAD);
  expect.add({ 4, 3 }, CellState::DEAD);
  expect.add({ 4, 4 }, CellState::DEAD);

  ASSERT_TRUE(expect.isSilent());
}

TEST(UniverseTests, iterator_should_only_traverse_alive_cells) {
  Universe sut;
  sut.add({ 0, 0 }, CellState::ALIVE);
  sut.add({ 0, 1 }, CellState::ALIVE);
  sut.add({ 0, 2 }, CellState::ALIVE);
  set<Point> expect{ { 0, 0 }, { 0, 1 }, { 0, 2 } };

  set<Point> locations;
  for (const auto &cell : sut) {
    ASSERT_FALSE(cell.isDead());
    locations.insert(cell.pos());
  }
  ASSERT_EQ(expect, locations);
}

TEST(UniverseTests, empty_university_should_equal) {
  ASSERT_EQ(Universe(), Universe());
}

TEST(UniverseTests, equality_one_cell_university) {
  Universe lhs, rhs;
  lhs.add({ 0, 0 }, CellState::ALIVE);
  rhs.add({ 0, 0 }, CellState::ALIVE);

  ASSERT_EQ(lhs, rhs);
}

TEST(UniverseTests, dead_cell_should_have_no_effect_on_equality) {
  Universe lhs, rhs;
  lhs.add({ 0, 0 }, CellState::ALIVE);
  lhs.add({ 1, 0 }, CellState::DEAD);
  rhs.add({ 0, 0 }, CellState::ALIVE);

  ASSERT_EQ(lhs, rhs);
}

TEST(UniverseTests, serialization_test) {
  auto expect = "**\n*.\n*.\n";
  Universe sut;
  sut.add({ 0, 0 }, CellState::ALIVE);
  sut.add({ 1, 0 }, CellState::ALIVE);
  sut.add({ 0, 1 }, CellState::ALIVE);
  sut.add({ 0, 2 }, CellState::ALIVE);

  stringstream sstr;
  sstr << sut;

  ASSERT_EQ(expect, sstr.str());
}

TEST(UniverseTests, deserialization_test) {
  Universe expect;
  expect.add({ 0, 0 }, CellState::ALIVE);
  expect.add({ 1, 0 }, CellState::ALIVE);
  expect.add({ 0, 1 }, CellState::ALIVE);
  expect.add({ 0, 2 }, CellState::ALIVE);
  stringstream sstr;
  sstr << expect;

  Universe actual;
  sstr >> actual;

  ASSERT_EQ(expect, actual);
}

TEST(UniverseTests, glider) {
  Universe origin = Universe::glider();

  Universe next;
  origin.nextGeneration(next);

  ASSERT_TRUE(next.alive({ 1, 0 }));
  ASSERT_TRUE(next.alive({ 1, 2 }));
  ASSERT_TRUE(next.alive({ 2, 1 }));
  ASSERT_TRUE(next.alive({ 2, 2 }));
  ASSERT_TRUE(next.alive({ 3, 1 }));
}
