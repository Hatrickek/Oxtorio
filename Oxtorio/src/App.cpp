#include <Core/EntryPoint.h>
#include <Core/Systems/HotReloadableScenes.h>

#include "OxtorioLayer.h"

namespace Ox {
class OxtorioApp : public Ox::Application {
public:
  OxtorioApp(const Ox::AppSpec& spec) : Application(spec) { }
};

Application* create_application(ApplicationCommandLineArgs args) {
  AppSpec spec;
  spec.name = "Oxtorio";
  spec.working_directory = std::filesystem::current_path().string();
  spec.command_line_args = args;
  spec.resources_path = "Assets";

  const auto app = new OxtorioApp(spec);
  app->push_layer(new Oxtorio::OxtorioLayer());

  return app;
}
}
