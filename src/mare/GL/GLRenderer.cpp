#include "mare/GL/GLRenderer.hpp"
#include "mare/Application.hpp"

// Standard Library
#include <iostream>

namespace mare
{
// Debug functions
std::string GLRenderer::debug_source_string(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "WINDOW_SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "SHADER_COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "THIRD_PARTY";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "APPLICATION";
    case GL_DEBUG_SOURCE_OTHER:
        return "OTHER";
    }
    return "INVALID_SOURCE";
}
std::string GLRenderer::debug_type_string(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        return "ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "DEPRICATED BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "UNDEFINED BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "PERFORMANCE";
    case GL_DEBUG_TYPE_OTHER:
        return "OTHER";
    case GL_DEBUG_TYPE_MARKER:
        return "MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP:
        return "PUSH GROUP";
    case GL_DEBUG_TYPE_POP_GROUP:
        return "POP GROUP";
    }
    return "INVALID_TYPE";
}
std::string GLRenderer::debug_severity_string(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        return "***HIGH***";
    case GL_DEBUG_SEVERITY_MEDIUM:
        return "**MEDIUM**";
    case GL_DEBUG_SEVERITY_LOW:
        return "*LOW*";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return "NOTIFICATION";
    }
    return "INVALID_SEVERITY";
}
void GLAPIENTRY
GLRenderer::debug_message_callback(GLenum source,
                                   GLenum type,
                                   GLuint id,
                                   GLenum severity,
                                   GLsizei length,
                                   const GLchar *message,
                                   const void *userParam)
{
    auto debug_state = info.debug_mode;
    if (debug_state[0] && severity == GL_DEBUG_SEVERITY_HIGH)
    {
        std::cerr << "GL_" << debug_source_string(source) << "_" << debug_type_string(type) << ": "
                  << debug_severity_string(severity) << " - " << message << std::endl;
    }
    if (debug_state[1] && severity == GL_DEBUG_SEVERITY_MEDIUM)
    {
        std::cerr << "GL_" << debug_source_string(source) << "_" << debug_type_string(type) << ": "
                  << debug_severity_string(severity) << " - " << message << std::endl;
    }
    if (debug_state[2] && severity == GL_DEBUG_SEVERITY_LOW)
    {
        std::cerr << "GL_" << debug_source_string(source) << "_" << debug_type_string(type) << ": "
                  << debug_severity_string(severity) << " - " << message << std::endl;
    }
    if (debug_state[3] && severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        std::cerr << "GL_" << debug_source_string(source) << "_" << debug_type_string(type) << ": "
                  << debug_severity_string(severity) << " - " << message << std::endl;
    }
}

void GLRenderer::start_process(Application *app_pointer)
{
    running = true;
    m_app_pointer = app_pointer;

    if (!glfwInit())
    {
        std::cerr << "GLFW Failed to initialize." << std::endl;
        running = false;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minor_version);
    if (info.debug_mode.any())
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, info.samples);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(info.window_width, info.window_height, "Untitled", info.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to open window." << std::endl;
        running = false;
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, glfw_onResize);
    glfwSetKeyCallback(window, glfw_onKey);
    glfwSetMouseButtonCallback(window, glfw_onMouseButton);
    glfwSetCursorPosCallback(window, glfw_onMouseMove);
    glfwSetScrollCallback(window, glfw_onMouseWheel);
    if (!info.cursor)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    hz_resize_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    arrow_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    crosshair_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    if (info.vsync)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW failed to initialize." << std::endl;
    }

    if (info.debug_mode.any())
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debug_message_callback, this);
        std::cout << "GL VENDOR: " << glGetString(GL_VENDOR) << "\n";
        std::cout << "GL VERSION: " << glGetString(GL_VERSION) << "\n";
        std::cout << "GL RENDERER: " << glGetString(GL_RENDERER) << std::endl;
    }
    m_app_pointer->startup();
    info.current_time = glfwGetTime();
    do
    {
        double time = glfwGetTime();
        m_app_pointer->render(time, time - info.current_time);
        info.current_time = time;
        glfwPollEvents();
        glfwSwapBuffers(window);
    } while (running && !glfwWindowShouldClose(window));
    m_app_pointer->shutdown();
    glfwDestroyCursor(hz_resize_cursor);
    glfwDestroyCursor(arrow_cursor);
    glfwDestroyCursor(hand_cursor);
    glfwDestroyCursor(crosshair_cursor);
    glfwDestroyWindow(window);
    glfwTerminate();
}

Mesh *GLRenderer::GenTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    return mesh_factory.GenTriangle(v1, v2, v3);
}

BasicMaterial *GLRenderer::GenBasicMaterial()
{
    return material_factory.GenBasicMaterial();
}

Material *GLRenderer::GenMaterial(const char *directory)
{
    return material_factory.GenMaterial(directory);
}

// Renderer callback functions
void GLRenderer::glfw_onResize(GLFWwindow *window, int w, int h)
{
    m_app_pointer->resize_window(w, h);
    if (m_camera_pointer)
    {
        m_camera_pointer->set_aspect(info.window_aspect);
        m_camera_pointer->recalculate_projection();
    }
    m_app_pointer->on_resize(w, h);
}

void GLRenderer::glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    m_app_pointer->on_key(key, action);
}

void GLRenderer::glfw_onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        input.mouse_button.set(0, 1);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        input.mouse_button.set(0, 0);
    }
    if (m_camera_pointer)
    {
        m_camera_pointer->interpret_input();
    }
    m_app_pointer->on_mouse_button(button, action);
}

void GLRenderer::glfw_onMouseMove(GLFWwindow *window, double x, double y)
{
    glm::ivec2 old_pos = get_input().mouse_pos;
    get_input().mouse_pos = glm::ivec2(x, y);
    get_input().mouse_vel = glm::ivec2(x, y) - old_pos;
    if (m_camera_pointer)
    {
        m_camera_pointer->interpret_input();
    }
    m_app_pointer->on_mouse_move(static_cast<int>(x), static_cast<int>(y));
}

void GLRenderer::glfw_onMouseWheel(GLFWwindow *window, double xoffset, double yoffset)
{
    m_app_pointer->on_mouse_wheel(static_cast<int>(yoffset));
}

GLFWwindow *GLRenderer::window = nullptr;
GLMeshFactory GLRenderer::mesh_factory{};
GLMaterialFactory GLRenderer::material_factory{};

} // namespace mare
