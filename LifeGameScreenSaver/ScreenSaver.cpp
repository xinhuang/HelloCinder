#include "ScreenSaver.h"

using namespace ci;
using namespace ci::app;

ScreenSaver::~ScreenSaver() {}

void ScreenSaver::prepareSettings(Settings* s) {
  base::prepareSettings(s);
  s->setFullScreen(true, FullScreenOptions());
}

void ScreenSaver::setup() {
  base::setup();
  hideCursor();
}

void ScreenSaver::mouseMove(MouseEvent event) {
  base::mouseMove(event);
  showCursor();
  quit();
}
