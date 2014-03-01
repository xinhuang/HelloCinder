#pragma once

#include <cinder/Cinder.h>
#include <cinder/app/AppBasic.h>

#include <memory>

class NetworkXApp : public ci::app::AppBasic {
public:
  NetworkXApp();
  ~NetworkXApp();

  void setup() final;

  void update() final;
  void draw() final;

  void keyUp(ci::app::KeyEvent e) final;
  void resize() final;
  void mouseDrag(ci::app::MouseEvent e) final;
  void mouseDown(ci::app::MouseEvent e) final;
  void mouseUp(ci::app::MouseEvent e) final;

private:
  struct Data;
  std::unique_ptr<Data> d;
};
