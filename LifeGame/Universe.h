#pragma once

#include "Cell.h"
#include "Rule.h"

#include <unordered_set>
#include <functional>

class Universe {
  using container_t = std::unordered_set<Cell, hash<Cell> >;

public:
  Universe() {}
  Universe &operator=(Universe &&v) {
    rule_ = std::move(v.rule_);
    cells_ = std::move(v.cells_);
    return *this;
  }

  bool isSilent() const;
  const Cell &operator[](const Point &pos) const;

  void add(const Cell &cell);
  void add(const Point &pos, CellState state) {
    add(Cell{ pos, state });
  }

  Universe nextGeneration() const;

  class const_iterator {
  public:
    const_iterator(const container_t &container) : container_(container) {
      pointer_ = container_.begin();
      while (pointer_ != container_.end() && pointer_->isDead()) {
        ++pointer_;
      }
    }

    const_iterator(const const_iterator &v)
        : container_(v.container_), pointer_(v.pointer_) {}

    const_iterator operator++() {
      ++pointer_;
      while (pointer_ != container_.end() && pointer_->isDead()) {
        ++pointer_;
      }
      return *this;
    }

    friend bool operator!=(const const_iterator &lhs,
                           const const_iterator &rhs) {
      return lhs.pointer_ != rhs.pointer_;
    }

    Cell operator*() const { return *pointer_; }

    const_iterator end() const {
      return const_iterator{ container_, container_.end() };
    }

  private:
    const_iterator(const container_t &container,
                   const container_t::const_iterator &iterator)
        : container_(container), pointer_(iterator) {}

  private:
    const container_t &container_;
    container_t::const_iterator pointer_;
  };

  const_iterator begin() const {
    return { cells_ };
  }
  const_iterator end() const { return const_iterator(cells_).end(); }

private:
  Universe(int size) { cells_.reserve(size); }
  std::vector<Point> neighbors(const Point &pos) const;
  std::vector<Cell> neighborCells(const Cell &pos) const;
  void createIfNonexists(const Point &p);

private:
  Rule rule_;
  container_t cells_;
};
