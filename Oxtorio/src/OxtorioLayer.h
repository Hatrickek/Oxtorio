#pragma once
#include <Core/Layer.h>

#include "Core/Systems/HotReloadableScenes.h"

#include "Scene/Scene.h"
#include "UI/RuntimeConsole.h"
#include "UI/UI.h"

namespace Oxtorio {
class OxtorioLayer : public Ox::Layer {
public:
  bool blocking_player_input = false;
  bool free_camera = false;

  OxtorioLayer();
  ~OxtorioLayer() override = default;
  void on_attach(Ox::EventDispatcher& dispatcher) override;
  void on_detach() override;
  void on_update(const Ox::Timestep& delta_time) override;
  void on_imgui_render() override;

  static OxtorioLayer* get() { return s_instance; }

  static std::string get_assets_path(const std::string_view path) {
    return ("Assets" / std::filesystem::path(path)).string();
  }

private:
  void load_scene();
  bool on_scene_reload(Ox::ReloadSceneEvent& e);

private:
  static OxtorioLayer* s_instance;

  Ox::EventDispatcher m_dispatcher;

  // Scene
  Ox::Shared<Ox::Scene> m_scene;

  //UI
  Ox::RuntimeConsole runtime_console;
};
}
