#include "OxtorioLayer.h"

#include <imgui.h>
#include <Scene/SceneSerializer.h>

#include <Render/RendererConfig.h>
#include <Core/Application.h>
#include <Render/RenderPipeline.h>
#include <UI/UI.h>
#include <Modules/ModuleUtil.h>

namespace Oxtorio {
OxtorioLayer* OxtorioLayer::s_instance = nullptr;

OxtorioLayer::OxtorioLayer() : Layer("Game Layer") {
  s_instance = this;
}

void OxtorioLayer::on_attach(Ox::EventDispatcher& dispatcher) {
  dispatcher.sink<Ox::ReloadSceneEvent>().connect<&OxtorioLayer::on_scene_reload>(*this);
  Ox::ModuleUtil::load_module("OxtorioModule", "OxtorioModule");
  load_scene();
}

void OxtorioLayer::on_detach() {
  Ox::ModuleUtil::unload_module("OxtorioModule");
}

void OxtorioLayer::on_update(const Ox::Timestep& delta_time) {
  m_scene->on_runtime_update(delta_time);
}

void OxtorioLayer::on_imgui_render() {
  m_scene->on_imgui_render(Ox::Application::get_timestep());

  UI::draw_runtime_console(runtime_console, &blocking_player_input);
  UI::draw_fps_counter();
}

void OxtorioLayer::load_scene() {
  m_scene = Ox::create_shared<Ox::Scene>();
  const Ox::SceneSerializer serializer(m_scene);
  serializer.deserialize(get_assets_path("Scenes/Main.oxscene"));

  m_scene->on_runtime_start();
}

bool OxtorioLayer::on_scene_reload(Ox::ReloadSceneEvent&) {
  load_scene();
  OX_CORE_INFO("Scene reloaded.");
  return true;
}
}
