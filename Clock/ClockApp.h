#pragma once

#include <cinder/cinder.h>
#include <cinder/app/AppBasic.h>
#include <cinder/gl/TextureFont.h>
#include <cinder/Timer.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

#include <iomanip>
using namespace std;

#include <boost/date_time.hpp>
using namespace boost::posix_time;

class ClockApp : public AppBasic {
public:
  ClockApp() : _dragging(false) {}

  void prepareSettings(Settings *settings) override;
  void setup() override;
  void draw() override;

  void keyUp(KeyEvent e) override {
    if (e.getCode() == KeyEvent::KEY_ESCAPE) {
      quit();
    }
  }

  void mouseUp(MouseEvent e) override {
    if (_dragging) {
      _dragging = false;
    }
  }

  void mouseDrag(MouseEvent e) override {
    if (_dragging) {
      auto delta = e.getPos() - _mouseDownPos;
      setWindowPos(getWindowPos() + delta);
    }
  }

  void mouseDown(MouseEvent e) override {
    if (!_dragging) {
      _dragging = true;
      _mouseDownPos = e.getPos();
    }
  }

private:
  string currentTime() const {
    auto time = second_clock::local_time().time_of_day();
    stringstream sstr;
    sstr << setfill('0') << setw(2) << time.hours() << setw(1) << ":" << setw(2)
         << time.minutes() << setw(1) << ":" << setw(2) << time.seconds();
    return sstr.str();
  }

private:
  bool _dragging;
  Vec2f _mouseDownPos;
  TextureFontRef _font;
};
