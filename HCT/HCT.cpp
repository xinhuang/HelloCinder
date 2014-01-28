#include "stdafx.h"
#include "HCT.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

#include "../HCUI/HCUI.h"
#include "../HCUI/TextBox.h"
using namespace hcui;

using namespace std;

struct Hct::Impl {
  UiSystemPtr ui_;
};

Hct::Hct() : m(make_unique<Impl>()) {}

Hct::~Hct() {}

void Hct::setup() {
  m->ui_ = make_unique<UiSystem>();

  m->ui_->hook(*this);
}

void Hct::shutdown() {}

void Hct::draw() {
  gl::clear(Color::black());
  m->ui_->do_events(); 
}

void Hct::update() {}

CINDER_APP_BASIC(Hct, RendererGl)
