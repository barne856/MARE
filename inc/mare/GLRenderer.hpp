#ifndef GLRENDERER
#define GLRENDERER

// OpenGL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
// Renderer
#include "mare/Renderer.hpp"

namespace mare
{
class GLRenderer : public Renderer
{
public:
    void renderer_process(Application *app_pointer) override;

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