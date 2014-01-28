#include "stdafx.h"

#include "RollApp.h"

void RollApp::setup() {
  _font = TextureFont::create(Font("Consolas", FontSize));
  _candidates = loadCandidates("candidates.txt");

  auto engine = mt19937{ random_device()() };
  auto dist = uniform_int_distribution<int>(0, _candidates.size() - 1);
  _rand = [=]() mutable { return dist(engine); };
}

vector<string> RollApp::loadCandidates(const string &path) const {
  if (boost::filesystem::exists(path)) {
    ifstream ifs(path);
    string line;
    vector<string> candidates;
    while (getline(ifs, line)) {
      candidates.push_back(line);
    }
    return candidates;
  } else {
    auto defaultCandidates =
        vector<string>{ "Zhaoli",       "Pengfei",  "CYan",    "Billy",
                        "Mike",         "Yinghua",  "Xinghua", "Difei",
                        "Renchu",       "Wang Wei", "Lin Dan", "Xin Huang",
                        "Boning Zhang", };
    ofstream ofs(path);
    for (const auto &item : defaultCandidates) {
      ofs << item << "\n";
    }
    return defaultCandidates;
  }
}

void RollApp::draw() {
  gl::clear(_bgColor);
  for (size_t i = 0; i < _angels.size(); ++i) {
    _font->drawString(_angels[i], rectOfLine(i));
  }

  if (isRolling())
    draw(currentName(), rollingRect());
  else
    draw("_", rollingRect());
}

void RollApp::update() {
  if (_rollTimer.getSeconds() >= RandomDuration && isRolling()) {
    _rollTimer.stop();
    _speed = 0;
    stopRoll();
  }

  _accum += _speed;
  if (_accum > 10) {
    roll();
    _accum -= 10;
    _speed -= 0.1;
  }
}

void RollApp::keyUp(KeyEvent e) {
  switch (e.getCode()) {
  case KeyEvent::KEY_ESCAPE:
    quit();

  case KeyEvent::KEY_SPACE:
    startRoll();
    break;
  }
}

void RollApp::startRoll() {
  if (_rollTimer.isStopped()) {
    _rollTimer.start();
    _speed = InitialSpeed;
    _bgColor = Color::black();
  }
}

void RollApp::stopRoll() {
  _rollTimer.stop();
  _speed = 0;
  _bgColor = Color::black();
  _angels.push_back(currentName());
  clearCurrent();
}

CINDER_APP_BASIC(RollApp, RendererGl)
