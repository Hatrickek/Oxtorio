#include "Module.h"

#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <Scripting/LuaHelpers.h>

#include "Components.h"

namespace Ox {
void OxtorioModule::register_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) {
  entt::locator<entt::meta_ctx>::reset(ctx); // set local meta context

  state->new_enum<OreType>(
    "OreType", {
      ENUM_FIELD(OreType, None),
      ENUM_FIELD(OreType, Iron),
      ENUM_FIELD(OreType, Stone),
      ENUM_FIELD(OreType, Copper)
    });

  REGISTER_COMPONENT_CTOR(state, PlaceableComponent, CTOR(bool), CFIELD(PlaceableComponent, placed));
  REGISTER_COMPONENT_CTOR(state, MinerComponent, CTOR(float, OreType), CFIELD(MinerComponent, speed) CFIELD(MinerComponent, type));
  REGISTER_COMPONENT_CTOR(state, OreComponent, CTOR(OreType), CFIELD(OreComponent, type));
  REGISTER_COMPONENT_CTOR(state, BeltComponent, CTOR(float), CFIELD(BeltComponent, speed));
  REGISTER_COMPONENT_CTOR(state, PickableComponent, CTOR(bool), CFIELD(PickableComponent, picked));
}

void OxtorioModule::unregister_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) {
  entt::meta_reset<PlaceableComponent>();
  entt::meta_reset<MinerComponent>();
  entt::meta_reset<OreComponent>();
  entt::meta_reset<BeltComponent>();
  entt::meta_reset<PickableComponent>();
}
}
