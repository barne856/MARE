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
    glm::vec3 raycast(Camera* camera) override;
    glm::vec3 raycast(Camera* camera, glm::ivec2 screen_coords) override;

    // Buffers
    virtual Scoped<Buffer<float>> GenFloatBuffer(std::vector<float> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<int>> GenIntBuffer(std::vector<int> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<unsigned int>> GenIndexBuffer(std::vector<unsigned int> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<bool>> GenBoolBuffer(std::vector<bool> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<glm::vec2>> GenVec2Buffer(std::vector<glm::vec2> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<glm::vec3>> GenVec3Buffer(std::vector<glm::vec3> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<glm::vec4>> GenVec4Buffer(std::vector<glm::vec4> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<glm::mat2>> GenMat2Buffer(std::vector<glm::mat2> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<glm::mat3>> GenMat3Buffer(std::vector<glm::mat3> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;
    virtual Scoped<Buffer<glm::mat4>> GenMat4Buffer(std::vector<glm::mat4> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0) override;

    // Textures
    virtual Scoped<Texture2D> GenTexture2D(const char *image_filepath) override;

    // Render States
    virtual Scoped<RenderState> GenRenderState() override;

    // Shaders
    virtual Scoped<Shader> GenShader(const char *directory) override;

    // Rendering Simple Meshes with no Composite or Instanced Meshes (Composite and Instanced Meshes are rendered by themselves)
    virtual void render_simple_mesh(const Layer *layer, const SimpleMesh *mesh, Material *material) override;
    virtual void render_simple_mesh(const Layer *layer, const SimpleMesh *mesh, Material *material, const glm::mat4 &parent_model) override;
    virtual void render_simple_mesh(const Layer *layer, const SimpleMesh *mesh, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) override;

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