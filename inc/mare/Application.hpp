#ifndef APPLICATION
#define APPLICATION

// Renderer
#include "mare/Renderer.hpp"

namespace mare
{

enum class CURSOR; // forward declaration

class Application
{
public:
    virtual ~Application() {}

    // Application driver function
    void run(Application *app_pointer);

    // App and Window Functions
    static inline Application *get_app() { return m_app_pointer; }
    static inline RendererInfo &get_info() { return m_renderer->get_info(); }
    static inline RendererInput &get_input() { return m_renderer->get_input(); }
    static inline void set_window_title(const char *title)
    {
        m_renderer->set_window_title(title);
    }
    static inline void set_cursor(CURSOR type)
    {
        m_renderer->set_cursor(type);
    }

    // Application main functions
    virtual void init(RendererInfo &info)
    {
        info.name = RendererName::OpenGL;
        info.window_width = 800;
        info.window_height = 600;
        info.window_aspect = 800.0f / 600.0f;
        info.major_version = 4;
        info.minor_version = 5;
        info.samples = 0;
        info.fullscreen = false;
        info.vsync = false;
        info.cursor = true;
        info.debug_mode = 0;
    }
    virtual void startup() = 0;
    virtual void render(double current_time, double delta_time) = 0;
    virtual void shutdown() = 0;
    void exit_appliction() { m_renderer->end_process(); }

    // Application callback functions
    virtual void on_resize(int w, int h) {}
    virtual void on_key(int key, int action) {}
    virtual void on_mouse_button(int button, int action) {}
    virtual void on_mouse_move(int x, int y) {}
    virtual void on_mouse_wheel(int pos) {}

    // Static Application data
    static Application *m_app_pointer;
    static Renderer *m_renderer;
};
} // namespace mare

#endif