#pragma once

#include <functional>
#include <cstdint>

template <typename T> struct hash;

struct Point {
  Point() : value{ 0 } {}
  Point(int x, int y) : x{ x }, y{ y } {}

  union {
    struct {
      int32_t x, y;
    };
    uint64_t value;
  };

  using hash = hash<Point>;
};

inline bool operator<(const Point &lhs, const Point &rhs) {
  return lhs.value < rhs.value;
}

inline bool operator==(const Point &lhs, const Point &rhs) {
  return lhs.value == rhs.value;
}

template <> struct hash<Point> {
  size_t operator()(const Point &p) const {
    return std::hash<uint64_t>()(p.value);
  }
};

enum class CellState {
  ALIVE,
  DEAD
};

class Cell {
public:
  Cell() : state_(CellState::DEAD) {}
  Cell(int x, int y) : pos_{ x, y }, state_(CellState::DEAD) {}
  Cell(const Point &p) : pos_(p), state_(CellState::DEAD) {}
  Cell(const Point &p, CellState s) : pos_(p), state_(s) {}

  bool isDead() const { return state_ == CellState::DEAD; }
  CellState state() const { return state_; }
  void setState(CellState s) { state_ = s; }
  const Point &pos() const { return pos_; }

  static const Cell DEAD_CELL;

private:
  Point pos_;
  CellState state_;
};

inline bool operator==(const Cell &lhs, const Cell &rhs) {
  return lhs.pos() == rhs.pos();
}

template <> struct hash<Cell> {
  size_t operator()(const Cell &c) const { return hash<Point>()(c.pos()); }
};
