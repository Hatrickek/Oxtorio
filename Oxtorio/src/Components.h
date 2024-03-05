#pragma once

#include <cstdint>

namespace Ox {
struct PlaceableComponent {
  bool placed;
};

enum class OreType : uint32_t { None, Iron, Stone, Copper };

struct MinerComponent {
  float speed = 5000.0f; // Miliseconds
  OreType type = OreType::None;
};

struct OreComponent {
  OreType type = OreType::None;
};

struct BeltComponent {
  float speed;
};

struct PickableComponent {
  bool picked;
};
}
