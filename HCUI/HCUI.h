#pragma once

#include <memory>
#include "Common.h"

namespace cinder {
namespace app {
class App;
}
}

namespace hcui {

class UiSystem {
  struct Impl;
  std::unique_ptr<Impl> m;

public:
  UiSystem();
  ~UiSystem();

  void do_events();
  void add(EntityPtr &entity);
  void hook(ci::app::App &app);
};

using UiSystemPtr = std::unique_ptr<UiSystem>;

template <typename T>
inline UiSystemPtr &operator+=(UiSystemPtr &ui, std::shared_ptr<T> &entity) {
  return ui += std::dynamic_pointer_cast<Entity, T>(entity);
}

inline UiSystemPtr &operator+=(UiSystemPtr &ui, EntityPtr &entity) {
  ui->add(entity);
  return ui;
}
}
