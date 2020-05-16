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
        void api_set_window_title(const char *title) override;
        void api_set_cursor(CURSOR type) override;
        void api_clear_color_buffer(glm::vec4 color) override;
        void api_clear_depth_buffer() override;
        void api_resize_window(int width, int height) override;
        void api_wireframe_mode(bool wireframe) override;
        void api_enable_depth_testing(bool enable) override;
        void api_enable_face_culling(bool enable) override;
        void api_enable_blending(bool enable) override;
        glm::vec3 api_raycast(Camera *camera) override;
        glm::vec3 api_raycast(Camera *camera, glm::ivec2 screen_coords) override;
        virtual void api_set_framebuffer(Framebuffer *framebuffer) override;

        // Textures
        virtual Scoped<Texture2D> api_gen_texture2D(const char *image_filepath) override;
        virtual Scoped<Texture2D> api_gen_texture2D(TextureType type, int width, int height) override;

        // Framebuffers
        virtual Scoped<Framebuffer> api_gen_framebuffer(int width, int height) override;

        // Shaders
        virtual Scoped<Shader> api_gen_shader(const char *directory) override;

        // Rendering Simple Meshes with no Composite or Instanced Meshes (Composite and Instanced Meshes are rendered by themselves)
        virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh, Material *material) override;
        virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh, Material *material, glm::mat4 parent_model) override;
        virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;
        virtual void api_bind_mesh_render_state(SimpleMesh *mesh, Material *material) override;
        virtual void api_destroy_mesh_render_states(SimpleMesh *mesh) override;
        virtual void api_push_mesh_geometry_buffer(SimpleMesh *mesh, Scoped<Buffer<float>> geometry_buffer) override;
        virtual void api_set_mesh_index_buffer(SimpleMesh *mesh, Scoped<Buffer<uint32_t>> index_buffer) override;

        // Compute Programs
        virtual void api_dispatch_compute(uint32_t x, uint32_t y, uint32_t z) override;

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