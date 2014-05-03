#pragma once

#include <random>
#include <type_traits>

template <typename T> struct distribution_traits {
  using type = typename std::conditional<
      std::is_integral<T>::value, std::uniform_int_distribution<T>,
      std::uniform_real_distribution<T> >::type;
};

class Random {
  std::mt19937 engine_ = std::mt19937{ std::random_device()() };

  static Random &instance() {
    static Random r;
    return r;
  }

  template <typename T> T _next() {
    auto dist = distribution_traits<T>::type();
    return dist(engine_);
  }

  template <typename T> T _next(const T &max) {
    auto dist = distribution_traits<T>::type({}, max);
    return dist(engine_);
  }

public:
  template <typename T> static T next() { return instance()._next<T>(); }

  template <typename T> static T next(const T &max) {
    return instance()._next<T>(max);
  }
};
