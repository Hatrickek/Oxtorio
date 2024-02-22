#pragma once

namespace Ox {
struct PlaceableComponent {
  bool placed;
};

enum class OreType : uint32_t { None, Iron, Stone, Copper };

struct MinerComponent {
  float speed;
  OreType type = OreType::None;
};

struct OreComponent {
  OreType type = OreType::None;
};

struct BeltComponent {
  float speed;
};
}
