#pragma once

//class Timer {
//  static double last_;
//
//public:
//  static void frameStart();
//  static double sinceLastFrame();
//};

class Timer {
public:
  virtual void reset() {}
  virtual double elapsed() const { return 0.0; }
};
