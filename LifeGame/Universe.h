#pragma once

#include "Cell.h"
#include "Rule.h"

#include <unordered_set>
#include <functional>
#include <iostream>
#include <functional>
#include <mutex>

#include <concurrent_unordered_set.h>

class Universe {
#if defined CONCURRENT_GENERATION
  using container_t = concurrency::concurrent_unordered_set<Cell, hash<Cell> >;
#else
  using container_t = std::unordered_set<Cell, hash<Cell> >;
#endif

public:
  Universe() {}
  Universe(const Universe &u) : rule_(u.rule_), cells_(u.cells_) {}
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
  void addn(const Cell& cell);
  void addn(const Point &pos, CellState state) {
    addn(Cell{ pos, state });
  }

  void nextGeneration(Universe& u) const;

  static Universe bigBang(int width, int height, double rate, const std::function<int(int)>& rand);
  static Universe glider();

  bool contains(const Cell& cell) const { return cells_.find(cell) != cells_.end(); }
  bool alive(const Point &pt) const { return !(*this)[pt].isDead(); }
  size_t size() const { return cells_.size(); }

  void clear() { cells_.clear(); }
  void swap(Universe& u) { cells_.swap(u.cells_); }

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
    const Cell* operator->() const { return &*pointer_; }

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
  std::vector<Point> neighbors(const Point &pos) const;
  std::vector<Cell> neighborCells(const Cell &pos) const;
  void createIfNonexists(const Point &p);
  bool Universe::tryAdd(const Cell& c);
  bool Universe::tryAdd(int x, int y) {
    return tryAdd({ x, y });
  }
  void sequentialNextGeneration(Universe& u) const;
  void parallelNextGeneration(Universe& u) const;

private:
  Rule rule_;
  container_t cells_;
};

bool operator==(const Universe &lhs, const Universe &rhs);

std::ostream& operator<<(std::ostream& os, const Universe& u);
std::istream& operator>>(std::istream& is, Universe& u);
