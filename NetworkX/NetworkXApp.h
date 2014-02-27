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

private:
  struct Data;
  std::unique_ptr<Data> d;
};

