#include "Module.h"

#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <Scripting/LuaHelpers.h>

#include "Components.h"

namespace Ox {
void OxtorioModule::register_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) {
  entt::locator<entt::meta_ctx>::reset(ctx); // set local meta context

  REGISTER_COMPONENT(state, PlaceableComponent, FIELD(PlaceableComponent, placed));

  state->new_enum<OreType>(
    "OreType", {
      ENUM_FIELD(OreType, Iron),
      ENUM_FIELD(OreType, Stone),
      ENUM_FIELD(OreType, Copper)
    });

  REGISTER_COMPONENT(state, MinerComponent, FIELD(MinerComponent, speed), FIELD(MinerComponent, type));

  REGISTER_COMPONENT_CTR(state, OreComponent, [](OreType type) {return OreComponent{type};}, FIELD(OreComponent, type));

  REGISTER_COMPONENT(state, BeltComponent, FIELD(BeltComponent, speed));
}

void OxtorioModule::unregister_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) {
  entt::meta_reset<PlaceableComponent>();
  entt::meta_reset<MinerComponent>();
  entt::meta_reset<OreComponent>();
  entt::meta_reset<BeltComponent>();
}
}
