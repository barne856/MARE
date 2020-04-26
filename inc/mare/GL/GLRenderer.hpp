#ifndef GLRENDERER
#define GLRENDERER

// OpenGL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
// MARE
#include "mare/Renderer.hpp"

namespace mare
{

class GLRenderer : public Renderer
{
public:
    void run() final;
    void init_renderer() final;
    void start_renderer() final;

    // Renderer Commands
    void set_window_title(const char *title) override;
    void set_cursor(CURSOR type) override;
    void clear_color_buffer(glm::vec4 color) override;
    void clear_depth_buffer() override;
    void resize_window(int width, int height) override;
    void wireframe_mode(bool wireframe) override;
    void enable_depth_testing(bool enable) override;
    void enable_face_culling(bool enable) override;
    void enable_blending(bool enable) override;
    glm::vec3 raycast(Layer *layer) override;
    glm::vec3 raycast(Layer *layer, glm::ivec2 screen_coords) override;

    // Textures
    virtual Scoped<Texture2DBuffer> GenTexture2DBuffer(const char *image_filepath) override;

    // Framebuffers
    virtual Scoped<Framebuffer> GenFramebuffer(int width, int height) override;

    // Shaders
    virtual Scoped<Shader> GenShader(const char *directory) override;

    // Rendering Simple Meshes with no Composite or Instanced Meshes (Composite and Instanced Meshes are rendered by themselves)
    virtual void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material) override;
    virtual void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material, glm::mat4 &parent_model) override;
    virtual void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;
    virtual void bind_mesh_render_state(SimpleMesh *mesh, Material *material) override;
    virtual void destroy_mesh_render_states(SimpleMesh *mesh) override;
    virtual void push_mesh_geometry_buffer(SimpleMesh *mesh, Scoped<Buffer<float>> geometry_buffer) override;
    virtual void set_mesh_index_buffer(SimpleMesh *mesh, Scoped<Buffer<uint32_t>> index_buffer) override;

private:
    static GLFWwindow *window;

    // Debug functions
    static void GLAPIENTRY debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
    static std::string debug_source_string(GLenum source);
    static std::string debug_type_string(GLenum type);
    static std::string debug_severity_string(GLenum severity);

    // Input callback functions
    static void glfw_onResize(GLFWwindow *window, int w, int h);
    static void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void glfw_onMouseButton(GLFWwindow *window, int button, int action, int mods);
    static void glfw_onMouseMove(GLFWwindow *window, double x, double y);
    static void glfw_onMouseWheel(GLFWwindow *window, double xoffset, double yoffset);

    // Cursors
    GLFWcursor *hz_resize_cursor;
    GLFWcursor *arrow_cursor;
    GLFWcursor *hand_cursor;
    GLFWcursor *crosshair_cursor;
};
} // namespace mare

#endif