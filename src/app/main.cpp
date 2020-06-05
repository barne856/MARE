// MARE
#include "mare/GL/GLRenderer.hpp"

// APP
#include "app/Scenes/SampleParticleScene.hpp"
#include "app/Scenes/SampleShadowScene.hpp"
#include "app/Scenes/SampleWidgetsScene.hpp"

class Sandbox : public mare::GLRenderer {
public:
  void init_info() override {
    info.window_title = "Sandbox";     // Window title
    info.window_width = 1280;          // window width in pixels
    info.window_height = 720;          // window height in pixels
    info.window_aspect = 16.0f / 9.0f; // window aspect ratio
    info.samples = 0;                  // antialiasing samples
    info.fullscreen = false;           // render in fullscreen mode?
    info.vsync = true;                 // render in double buffered vsync mode?
    info.debug_mode = 1; // 0000 == off, 0001 == high, 0010 == med, 0100 == low,
                         // 1000 == notification
  }
  void startup() override {
    using namespace mare;
    auto layer = gen_scene<SampleWidgetsScene>();
    load_scene(0);
  }
  void shutdown() override {}
};

int main(int argc, char **argv) { return mare::launch<Sandbox>(); }