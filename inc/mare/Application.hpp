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
    static inline Camera *get_camera() { return m_renderer_pointer->get_camera(); }
    static inline void set_window_title(const char *title)
    {
        m_renderer_pointer->set_window_title(title);
    }
    static inline void resize_window(int width, int height)
    {
        m_renderer_pointer->resize_window(width, height);
    }
    static inline void set_camera(Camera *camera)
    {
        Renderer::set_camera(camera);
    }
    static inline void set_cursor(CURSOR type)
    {
        m_renderer_pointer->set_cursor(type);
    }
    static inline void clear_color_buffer(glm::vec4 color)
    {
        m_renderer_pointer->clear_color_buffer(color);
    }
    static inline void wireframe_mode(bool wireframe)
    {
        m_renderer_pointer->wireframe_mode(wireframe);
    }

    // Buffers
    static Buffer<float>* GenFloatBuffer()
    {
        return m_renderer_pointer->GenFloatBuffer();
    }
    static Buffer<glm::mat4>* GenMat4Buffer()
    {
        return m_renderer_pointer->GenMat4Buffer();
    }

    // RenderState
    static RenderState* GenRenderState()
    {
        return m_renderer_pointer->GenRenderState();
    }

    // Meshes
    static void render_mesh(Mesh* mesh, Material* material)
    {
        m_renderer_pointer->render_mesh(mesh, material);
    }
    static void render_mesh(Mesh* mesh, Material* material, glm::mat4 parent_model)
    {
        m_renderer_pointer->render_mesh(mesh, material, parent_model);
    }
    static void render_mesh(Mesh* mesh, Material* material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4>* models)
    {
        m_renderer_pointer->render_mesh(mesh, material, parent_model, instance_count, models);
    }

    // Materials
    static Shader* GenShader(const char *directory)
    {
        return m_renderer_pointer->GenShader(directory);
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
    virtual void on_key(const RendererInput &input) {}
    virtual void on_mouse_button(const RendererInput &input) {}
    virtual void on_mouse_move(const RendererInput &input) {}
    virtual void on_mouse_wheel(const RendererInput &input) {}

private:
    // Static Application data
    static Application *m_app_pointer;
    static Renderer *m_renderer_pointer;
};
} // namespace mare

#endif