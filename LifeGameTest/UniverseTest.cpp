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

  auto now = sut.nextGeneration();

  auto cell = now[{ 0, 1 }];
  ASSERT_FALSE(cell.isDead());
}

TEST(UniverseTests, birth_if_3_neighbors) {
  Universe sut;
  sut.add({ 0, 0 }, CellState::ALIVE);
  sut.add({ 2, 0 }, CellState::ALIVE);
  sut.add({ 1, 1 }, CellState::ALIVE);

  auto expect = sut.nextGeneration();

  ASSERT_TRUE(!expect.isSilent());
}

TEST(UniverseTests, square_is_a_stable_pattern) {
  Universe sut;
  sut.add({ 3, 3 }, CellState::ALIVE);
  sut.add({ 3, 4 }, CellState::ALIVE);
  sut.add({ 4, 3 }, CellState::ALIVE);
  sut.add({ 4, 4 }, CellState::ALIVE);

  auto expect = sut.nextGeneration();

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
