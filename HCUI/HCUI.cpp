#include "stdafx.h"
#include "HCUI.h"

using namespace std;

#include <cinder/app/App.h>
using namespace ci::app;

namespace hcui {

struct UiSystem::Impl {
  vector<EntityPtr> entities_;
};

UiSystem::UiSystem() : m(make_unique<Impl>()) {}

UiSystem::~UiSystem() {}

void UiSystem::hook(App& app) {

}

void UiSystem::add(EntityPtr &entity) {
  m->entities_.push_back(entity);
}

void UiSystem::do_events() {
  for (auto& e : m->entities_)
    e->do_events();
}
}
