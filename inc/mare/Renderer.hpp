#ifndef RENDERER
#define RENDERER

// Standard Library
#include <bitset>
// MARE
#include "mare/Camera.hpp"
#include "mare/SimpleMesh.hpp"
#include "mare/Buffer.hpp"
#include "mare/RenderState.hpp"
#include "mare/Material.hpp"
// External Libraries
#include "glm.hpp"

namespace mare
{

class Application; // forward declaration
class Scene;       // forward declaration

enum class RendererName
{
    OpenGL,
    DirectX, // not implemented yet
    Vulkan   // not implemented yet
};

enum class CURSOR
{
    ARROW,
    HZ_RESIZE,
    HAND,
    CROSSHAIRS
};

struct RendererInfo
{
    RendererName name{};
    int window_width{};          // window width in pixels
    int window_height{};         // window height in pixels
    float window_aspect{};       // window aspect ratio
    double current_time{};       // elapsed time in seconds
    int major_version{};         // major OpenGL version
    int minor_version{};         // minor OpenGL version
    int samples{};               // antialiasing samples
    bool wireframe = false;      // render in wireframe mode?
    bool fullscreen = false;     // render in fullscreen mode?
    bool vsync = false;          // render in double buffered vsync mode?
    bool cursor = true;          // render cursor?
    std::bitset<4> debug_mode{}; // 0000 == off, 0001 == high, 0010 == med, 0100 == low, 1000 == notification
};

struct RendererInput
{
    std::bitset<3> mouse_button{}; // 001 == left, 010 == right, 100 == middle
    glm::ivec2 mouse_pos{};        // window coordinates of mouse position (in pixels from top left corner)
    glm::ivec2 mouse_vel{};        // window coordinates of mouse velocity (in pixels from top left corner)
    bool W_PRESSED = false;
};

class Renderer
{
public:
    virtual ~Renderer() {}
    virtual void start_process(Application *app_pointer) = 0;
    static inline void end_process() { running = false; }
    static inline void set_camera(Camera *camera) { m_camera_pointer = camera; }
    static inline void set_scene(Scene *scene) { m_scene = scene; }

    static inline RendererInfo &get_info() { return info; }
    static inline RendererInput &get_input() { return input; }
    static inline Camera *get_camera() { return m_camera_pointer; }

    // Renderer Commands
    virtual void set_window_title(const char *title) = 0;
    virtual void set_cursor(CURSOR type) = 0;
    virtual void clear_color_buffer(glm::vec4 color) = 0;
    virtual void resize_window(int width, int height) = 0;
    virtual void wireframe_mode(bool wireframe) = 0;
    virtual void enable_depth_testing(bool enable) = 0;
    virtual void clear_depth_buffer() = 0;
    virtual void enable_face_culling(bool enable) = 0;

    // Buffers
    virtual Buffer<float> *GenFloatBuffer(unsigned int count) = 0;
    virtual Buffer<int> *GenIntBuffer(unsigned int count) = 0;
    virtual Buffer<unsigned int> *GenIndexBuffer(unsigned int count) = 0;
    virtual Buffer<bool> *GenBoolBuffer(unsigned int count) = 0;
    virtual Buffer<glm::mat4> *GenMat4Buffer(unsigned int count) = 0;
    virtual Buffer<glm::mat3> *GenMat3Buffer(unsigned int count) = 0;
    virtual Buffer<glm::mat2> *GenMat2Buffer(unsigned int count) = 0;
    virtual Buffer<glm::vec2> *GenVec2Buffer(unsigned int count) = 0;
    virtual Buffer<glm::vec3> *GenVec3Buffer(unsigned int count) = 0;
    virtual Buffer<glm::vec4> *GenVec4Buffer(unsigned int count) = 0;

    // Render States
    virtual RenderState<float> *GenFloatRenderState() = 0;
    virtual RenderState<glm::vec2> *GenVec2RenderState() = 0;
    virtual RenderState<glm::vec3> *GenVec3RenderState() = 0;
    virtual RenderState<glm::vec4> *GenVec4RenderState() = 0;

    // Rendering Meshes
    virtual void render_float_mesh(SimpleMesh<float> *mesh, Material *material) = 0;
    virtual void render_float_mesh(SimpleMesh<float> *mesh, Material *material, glm::mat4 parent_model) = 0;
    virtual void render_float_mesh(SimpleMesh<float> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
    virtual void render_vec2_mesh(SimpleMesh<glm::vec2> *mesh, Material *material) = 0;
    virtual void render_vec2_mesh(SimpleMesh<glm::vec2> *mesh, Material *material, glm::mat4 parent_model) = 0;
    virtual void render_vec2_mesh(SimpleMesh<glm::vec2> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
    virtual void render_vec3_mesh(SimpleMesh<glm::vec3> *mesh, Material *material) = 0;
    virtual void render_vec3_mesh(SimpleMesh<glm::vec3> *mesh, Material *material, glm::mat4 parent_model) = 0;
    virtual void render_vec3_mesh(SimpleMesh<glm::vec3> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
    virtual void render_vec4_mesh(SimpleMesh<glm::vec4> *mesh, Material *material) = 0;
    virtual void render_vec4_mesh(SimpleMesh<glm::vec4> *mesh, Material *material, glm::mat4 parent_model) = 0;
    virtual void render_vec4_mesh(SimpleMesh<glm::vec4> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;

    // Shaders
    virtual Shader *GenShader(const char *directory) = 0;

    // Coordinate conversions
    // get normalized device coordinates from window coordinates
    static inline void normalized_device_coordinates(int x, int y, float &nx, float &ny)
    {
        nx = float(2 * x) / float(info.window_width) - 1.0f;
        ny = 1.0f - float(2 * y) / float(info.window_height);
    }
    // get window coordinates from normalized device coordinates
    static inline void window_coordinates(float nx, float ny, int &x, int &y)
    {
        x = static_cast<int>(float(info.window_width) * (nx + 1.0f) / 2.0f);
        y = static_cast<int>(float(info.window_height) * (ny - 1.0f) / -2.0f);
    }

protected:
    static RendererInfo info;
    static RendererInput input;
    static Application *m_app_pointer;
    static Camera *m_camera_pointer;
    static bool running;
    static Scene *m_scene;
};
} // namespace mare

#endif