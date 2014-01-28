#pragma once

#include <vector>
#include <string>
#include <random>

using namespace std;

#include <cinder/cinder.h>
#include <cinder/app/AppBasic.h>
#include <cinder/gl/TextureFont.h>
#include <cinder/Timer.h>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class RollApp : public AppBasic {
  int _choosen;
  const float FontSize = 50.f;
  static const int RandomDuration = 5;
  const double InitialSpeed = 10.0;
  Timer _rollTimer;
  double _speed;
  double _accum;
  Color _bgColor;

  function<int()> _rand;
  shared_ptr<TextureFont> _font;
  vector<string> _angels;
  vector<string> _candidates;

public:
  void setup() override;
  void draw() override;
  void update() override;
  void keyUp(KeyEvent e) override;

private:
  void clearCurrent() { _choosen = -1; }
  string currentName() const { return _candidates[_choosen]; }
  bool isRolling() const { return !_rollTimer.isStopped(); }
  Rectf rollingRect() const { return rectOfLine(_angels.size()); }

  void draw(const string &text, const Rectf &rect) {
    _font->drawString(text, rect);
  }

  vector<string> loadCandidates(const string &path) const;

  Rectf rectOfLine(size_t i) const {
    return { 0.f,                     FontSize * i,
             (float)getWindowWidth(), (float)getWindowHeight() };
  }

  void startRoll();
  void roll() { _choosen = _rand(); }
  void stopRoll();
};
