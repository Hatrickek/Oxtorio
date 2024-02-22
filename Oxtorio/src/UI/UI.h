#pragma once
namespace Ox {
class RuntimeConsole;
}

namespace Oxtorio {
class UI {
public:
  static void draw_runtime_console(Ox::RuntimeConsole& console, bool* block_player_input);
  static void draw_fps_counter();
};
}
