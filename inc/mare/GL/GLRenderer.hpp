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
    void start_process(Application *app_pointer) override;

protected:
    void set_window_title(const char *title) override
    {
        glfwSetWindowTitle(window, title);
    }
    void set_cursor(CURSOR type) override
    {
        switch (type)
        {
        case CURSOR::ARROW:
            glfwSetCursor(window, arrow_cursor);
            break;

        case CURSOR::HZ_RESIZE:
            glfwSetCursor(window, hz_resize_cursor);
            break;

        case CURSOR::HAND:
            glfwSetCursor(window, hand_cursor);
            break;

        case CURSOR::CROSSHAIRS:
            glfwSetCursor(window, crosshair_cursor);
            break;
        }
    }
    void clear_color_buffer(glm::vec4 color) override
    {
        glClearBufferfv(GL_COLOR, 0, &color[0]);
    }
    void resize_window(int width, int height) override
    {
        info.window_width = width;
        info.window_height = height;
        info.window_aspect = float(info.window_width) / float(info.window_height);
        glViewport(0, 0, width, height);
    }
    void wireframe_mode(bool wireframe) override
    {
        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    // Buffers
    Buffer<float> *GenFloatBuffer() override;
    Buffer<glm::mat4>* GenMat4Buffer() override;

    // RenderState
    RenderState *GenRenderState() override;

    // Meshes
    void render_mesh(Mesh *mesh, Material *material) override;
    void render_mesh(Mesh *mesh, Material *material, glm::mat4 parent_model) override;
    void render_mesh(Mesh *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;

    // Shaders
    Shader* GenShader(const char *directory) override;

private:
    static GLFWwindow *window;

    GLenum GLDrawMethod(DrawMethod draw_method);

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