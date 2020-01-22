#ifndef APPLICATION
#define APPLICATION

// MARE
#include "mare/Scene.hpp"

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
    static inline void set_scene(Scene* scene)
    {
        Renderer::set_scene(scene);
    }
    static inline void set_cursor(CURSOR type)
    {
        m_renderer_pointer->set_cursor(type);
    }
    static inline void clear_color_buffer(glm::vec4 color)
    {
        m_renderer_pointer->clear_color_buffer(color);
    }
    static inline void clear_depth_buffer()
    {
        m_renderer_pointer->clear_depth_buffer();
    }
    static inline void wireframe_mode(bool wireframe)
    {
        m_renderer_pointer->wireframe_mode(wireframe);
    }
    static inline void enable_depth_testing(bool enable)
    {
        m_renderer_pointer->enable_depth_testing(enable);
    }
    static inline void enable_face_culling(bool enable)
    {
        m_renderer_pointer->enable_face_culling(enable);
    }

    // Buffers
    template <typename T>
    static Buffer<T> *GenBuffer(unsigned int count)
    {
        return nullptr;
    }
    template <>
    static Buffer<float> *GenBuffer<float>(unsigned int count)
    {
        return m_renderer_pointer->GenFloatBuffer(count);
    }
    template <>
    static Buffer<int> *GenBuffer<int>(unsigned int count)
    {
        return m_renderer_pointer->GenIntBuffer(count);
    }
    template <>
    static Buffer<unsigned int> *GenBuffer<unsigned int>(unsigned int count)
    {
        return m_renderer_pointer->GenIndexBuffer(count);
    }
    template <>
    static Buffer<bool> *GenBuffer<bool>(unsigned int count)
    {
        return m_renderer_pointer->GenBoolBuffer(count);
    }
    template <>
    static Buffer<glm::mat4> *GenBuffer<glm::mat4>(unsigned int count)
    {
        return m_renderer_pointer->GenMat4Buffer(count);
    }
    template <>
    static Buffer<glm::mat3> *GenBuffer<glm::mat3>(unsigned int count)
    {
        return m_renderer_pointer->GenMat3Buffer(count);
    }
    template <>
    static Buffer<glm::mat2> *GenBuffer<glm::mat2>(unsigned int count)
    {
        return m_renderer_pointer->GenMat2Buffer(count);
    }
    template <>
    static Buffer<glm::vec2> *GenBuffer<glm::vec2>(unsigned int count)
    {
        return m_renderer_pointer->GenVec2Buffer(count);
    }
    template <>
    static Buffer<glm::vec3> *GenBuffer<glm::vec3>(unsigned int count)
    {
        return m_renderer_pointer->GenVec3Buffer(count);
    }
    template <>
    static Buffer<glm::vec4> *GenBuffer<glm::vec4>(unsigned int count)
    {
        return m_renderer_pointer->GenVec4Buffer(count);
    }

    // Render State Generation
    template <typename T>
    static RenderState<T> *GenRenderState()
    {
        return nullptr;
    }
    template <>
    static RenderState<float> *GenRenderState<float>()
    {
        return m_renderer_pointer->GenFloatRenderState();
    }
    template <>
    static RenderState<glm::vec2> *GenRenderState<glm::vec2>()
    {
        return m_renderer_pointer->GenVec2RenderState();
    }
    template <>
    static RenderState<glm::vec3> *GenRenderState<glm::vec3>()
    {
        return m_renderer_pointer->GenVec3RenderState();
    }
    template <>
    static RenderState<glm::vec4> *GenRenderState<glm::vec4>()
    {
        return m_renderer_pointer->GenVec4RenderState();
    }

    // Meshes
    template <typename T>
    static void render_mesh(SimpleMesh<T> *mesh, Material *material) {}
    template <>
    static void render_mesh<float>(SimpleMesh<float> *mesh, Material *material)
    {
        m_renderer_pointer->render_float_mesh(mesh, material);
    }
    template <>
    static void render_mesh<glm::vec2>(SimpleMesh<glm::vec2> *mesh, Material *material)
    {
        m_renderer_pointer->render_vec2_mesh(mesh, material);
    }
    template <>
    static void render_mesh<glm::vec3>(SimpleMesh<glm::vec3> *mesh, Material *material)
    {
        m_renderer_pointer->render_vec3_mesh(mesh, material);
    }
    template <>
    static void render_mesh<glm::vec4>(SimpleMesh<glm::vec4> *mesh, Material *material)
    {
        m_renderer_pointer->render_vec4_mesh(mesh, material);
    }
    template <typename T>
    static void render_mesh(SimpleMesh<T> *mesh, Material *material, glm::mat4 parent_model) {}
    template <>
    static void render_mesh<float>(SimpleMesh<float> *mesh, Material *material, glm::mat4 parent_model)
    {
        m_renderer_pointer->render_float_mesh(mesh, material, parent_model);
    }
    template <>
    static void render_mesh<glm::vec2>(SimpleMesh<glm::vec2> *mesh, Material *material, glm::mat4 parent_model)
    {
        m_renderer_pointer->render_vec2_mesh(mesh, material, parent_model);
    }
    template <>
    static void render_mesh<glm::vec3>(SimpleMesh<glm::vec3> *mesh, Material *material, glm::mat4 parent_model)
    {
        m_renderer_pointer->render_vec3_mesh(mesh, material, parent_model);
    }
    template <>
    static void render_mesh<glm::vec4>(SimpleMesh<glm::vec4> *mesh, Material *material, glm::mat4 parent_model)
    {
        m_renderer_pointer->render_vec4_mesh(mesh, material, parent_model);
    }
    template <typename T>
    static void render_mesh(SimpleMesh<T> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) {}
    template <>
    static void render_mesh<float>(SimpleMesh<float> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
    {
        m_renderer_pointer->render_float_mesh(mesh, material, parent_model, instance_count, models);
    }
    template <>
    static void render_mesh<glm::vec2>(SimpleMesh<glm::vec2> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
    {
        m_renderer_pointer->render_vec2_mesh(mesh, material, parent_model, instance_count, models);
    }
    template <>
    static void render_mesh<glm::vec3>(SimpleMesh<glm::vec3> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
    {
        m_renderer_pointer->render_vec3_mesh(mesh, material, parent_model, instance_count, models);
    }
    template <>
    static void render_mesh<glm::vec4>(SimpleMesh<glm::vec4> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
    {
        m_renderer_pointer->render_vec4_mesh(mesh, material, parent_model, instance_count, models);
    }

    // Materials
    static Shader *GenShader(const char *directory)
    {
        return m_renderer_pointer->GenShader(directory);
    }

    // Application main functions
    virtual void init(RendererInfo &info)
    {
        info.name = RendererName::OpenGL;
        info.window_width = 1280;
        info.window_height = 720;
        info.window_aspect = 1280.0f / 720.0f;
        info.major_version = 4;
        info.minor_version = 5;
        info.samples = 16;
        info.fullscreen = false;
        info.vsync = false;
        info.cursor = true;
        info.debug_mode.set(0, 1);
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