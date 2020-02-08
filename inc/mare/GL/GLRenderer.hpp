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
    void init() override;
    void start_process() override;

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

    // Buffers
    Buffer<float> *GenFloatBuffer(unsigned int count) override;
    Buffer<int> *GenIntBuffer(unsigned int count) override;
    Buffer<unsigned int> *GenIndexBuffer(unsigned int count) override;
    Buffer<bool> *GenBoolBuffer(unsigned int count) override;
    Buffer<glm::mat4> *GenMat4Buffer(unsigned int count) override;
    Buffer<glm::mat3> *GenMat3Buffer(unsigned int count) override;
    Buffer<glm::mat2> *GenMat2Buffer(unsigned int count) override;
    Buffer<glm::vec2> *GenVec2Buffer(unsigned int count) override;
    Buffer<glm::vec3> *GenVec3Buffer(unsigned int count) override;
    Buffer<glm::vec4> *GenVec4Buffer(unsigned int count) override;

    // Textures
    Texture2D *GenTexture2D(const char *image_filepath) override;

    // Render States
    RenderState *GenRenderState() override;

    // Shaders
    Shader *GenShader(const char *directory) override;

    // Rendering Simple Meshes with no Composite or Instanced Meshes (Composite and Instanced Meshes are rendered by themselves)
    void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material) override;
    void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material, glm::mat4 parent_model) override;
    void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;

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