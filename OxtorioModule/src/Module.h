#pragma once

#define OX_BUILD_DLL

#include <entt/locator/locator.hpp>

#include <Modules/Linker.h>
#include <Modules/ModuleInterface.h>

#include <sol/state.hpp>

namespace Ox {
class OX_SHARED OxtorioModule : public ModuleInterface {
public:
  ~OxtorioModule() override = default;
  void register_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) override;
  void unregister_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) override;
};

CREATE_MODULE_FUNC {
  return new OxtorioModule;
}

}
