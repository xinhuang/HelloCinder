#include "Button.h"

using namespace std;

namespace ui {

struct Button::Data {};

Button::Button() : d(make_unique<Data>()) {}

Button::~Button() {}

}