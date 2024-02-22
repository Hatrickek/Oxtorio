#include "Module.h"

#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <Scripting/LuaHelpers.h>

#include "Components.h"

namespace Ox {
void OxtorioModule::register_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) {
  entt::locator<entt::meta_ctx>::reset(ctx); // set local meta context

  auto placeable_comp = NEW_TYPE_WITH_CTOR(state, PlaceableComponent, [](bool x) {return PlaceableComponent{x};})
  SET_COMPONENT_TYPE_ID(placeable_comp, PlaceableComponent);
  SET_TYPE_FIELD(placeable_comp, PlaceableComponent, placed);
  register_meta_component<PlaceableComponent>();

  state->new_enum<OreType>(
    "OreType",
    {
      {"Iron", OreType::Iron},
      {"Stone", OreType::Stone},
      {"Copper", OreType::Copper}
    });

  auto miner_comp = NEW_TYPE_WITH_CTOR(state, MinerComponent, [](float x) {return MinerComponent{x};})
  SET_COMPONENT_TYPE_ID(miner_comp, MinerComponent);
  SET_TYPE_FIELD(miner_comp, MinerComponent, speed);
  SET_TYPE_FIELD(miner_comp, MinerComponent, type);
  register_meta_component<MinerComponent>();

  auto ore_comp = NEW_TYPE_WITH_CTOR(state, OreComponent, [](OreType type) {return OreComponent{type};})
  SET_COMPONENT_TYPE_ID(ore_comp, OreComponent);
  SET_TYPE_FIELD(ore_comp, OreComponent, type);
  register_meta_component<OreComponent>();

  auto belt_comp = NEW_TYPE_WITH_CTOR(state, BeltComponent, [](float speed) {return BeltComponent{speed};})
  SET_COMPONENT_TYPE_ID(belt_comp, BeltComponent);
  SET_TYPE_FIELD(belt_comp, BeltComponent, speed);
  register_meta_component<BeltComponent>();
}

void OxtorioModule::unregister_components(sol::state* state, const entt::locator<entt::meta_ctx>::node_type& ctx) {
  entt::meta_reset<PlaceableComponent>();
  entt::meta_reset<MinerComponent>();
  entt::meta_reset<OreComponent>();
  entt::meta_reset<BeltComponent>();
}
}
