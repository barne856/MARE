#ifndef APPLICATION
#define APPLICATION

// MARE
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
    static inline RendererInfo &get_info() { return m_renderer_pointer->get_info(); }
    static inline RendererInput &get_input() { return m_renderer_pointer->get_input(); }
    static inline void set_window_title(const char *title)
    {
        m_renderer_pointer->set_window_title(title);
    }
    static inline void resize_window(int width, int height)
    {
        m_renderer_pointer->resize_window(width, height);
    }
    static inline void set_camera(Camera* camera)
    {
        m_renderer_pointer->set_camera(camera);
    }
    static inline void set_cursor(CURSOR type)
    {
        m_renderer_pointer->set_cursor(type);
    }
    static inline void clear_color_buffer(glm::vec4 color)
    {
        m_renderer_pointer->clear_color_buffer(color);
    }

    // Meshes
    static Mesh *GenTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
    {
        return m_renderer_pointer->GenTriangle(v1, v2, v3);
    }

    // Materials
    static BasicMaterial *GenBasicMaterial()
    {
        return m_renderer_pointer->GenBasicMaterial();
    }
    static Material* GenMaterial(const char* directory)
    {
        return m_renderer_pointer->GenMaterial(directory);
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
    void exit_appliction() { m_renderer_pointer->end_process(); }

    // Application callback functions
    virtual void on_resize(int width, int height) {}
    virtual void on_key(int key, int action) {}
    virtual void on_mouse_button(int button, int action) {}
    virtual void on_mouse_move(int x, int y) {}
    virtual void on_mouse_wheel(int pos) {}

private:
    // Static Application data
    static Application *m_app_pointer;
    static Renderer *m_renderer_pointer;
};
} // namespace mare

#endif