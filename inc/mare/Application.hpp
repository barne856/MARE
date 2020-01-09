#ifndef APPLICATION
#define APPLICATION

// OpenGL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
// Standard Library
#include <string>
#include <bitset>
// External Libraries
#include "glm.hpp"

namespace mare
{
enum class CURSOR
{
    ARROW,
    HZ_RESIZE,
    HAND,
    CROSSHAIRS
};

struct AppInfo
{
    int window_width {};          // window width in pixels
    int window_height {};         // window height in pixels
    float window_aspect {};    // window aspect ratio
    double current_time {};     // elapsed time in seconds
    int major_version {};         // major OpenGL version
    int minor_version {};         // minor OpenGL version
    int samples {};               // antialiasing samples
    bool wireframe = false;        // render in wireframe mode?
    bool fullscreen = false;       // render in fullscreen mode?
    bool vsync = false;            // render in double buffered vsync mode?
    bool cursor = true;            // render cursor?
    std::bitset<4> debug_mode {}; // 0000 == off, 0001 == high, 0010 == med, 0100 == low, 1000 == notification
};

struct AppInput
{
    std::bitset<3> mouse_button {};        // 001 == left, 010 == right, 100 == middle
    glm::ivec2 mouse_pos {}; // window coordinates of mouse position (in pixels from top left corner)
};

class Application
{
public:
    Application() {}
    virtual ~Application() {}

    // Application driver function
    void run(Application* app_pointer);

    // App and Window Functions
    static inline Application *get_app() { return m_app_pointer; }
    static inline GLFWwindow *get_window() { return m_app_pointer->window; }
    static inline AppInfo *get_info() { return &(m_app_pointer->info); }
    static inline AppInput *get_input() { return &input; }
    static void set_window_title(const char *title)
    {
        glfwSetWindowTitle(get_window(), title);
    }
    static inline void set_cursor(CURSOR type)
    {
        switch (type)
        {
        case CURSOR::ARROW:
            glfwSetCursor(m_app_pointer->window, m_app_pointer->arrow_cursor);
            break;

        case CURSOR::HZ_RESIZE:
            glfwSetCursor(m_app_pointer->window, m_app_pointer->hz_resize_cursor);
            break;

        case CURSOR::HAND:
            glfwSetCursor(m_app_pointer->window, m_app_pointer->hand_cursor);
            break;

        case CURSOR::CROSSHAIRS:
            glfwSetCursor(m_app_pointer->window, m_app_pointer->crosshair_cursor);
            break;
        }
    }

    // Coordinate conversions
    // get normalized device coordinates from window coordinates
    static inline void normalized_device_coordinates(int x, int y, float &nx, float &ny)
    {
        nx = float(2 * x) / float(m_app_pointer->info.window_width) - 1.0f;
        ny = 1.0f - float(2 * y) / float(m_app_pointer->info.window_height);
    }
    // get window coordinates from normalized device coordinates
    static inline void window_coordinates(float nx, float ny, int &x, int &y)
    {
        x = static_cast<int>(float(m_app_pointer->info.window_width) * (nx + 1.0f) / 2.0f);
        y = static_cast<int>(float(m_app_pointer->info.window_height) * (ny - 1.0f) / -2.0f);
    }

protected:
    // Static Application data
    static Application *m_app_pointer;
    static GLFWwindow *window;
    static AppInfo info;
    static AppInput input;

    // Application main functions
    virtual void init()
    {
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
    void exit_appliction() { running = false; }

    // Application callback functions
    virtual void on_resize(int w, int h)
    {
        info.window_width = w;
        info.window_height = h;
        info.window_aspect = float(info.window_width) / float(info.window_height);
        glViewport(0, 0, w, h);
    }
    virtual void on_key(int key, int action) {}
    virtual void on_mouse_button(int button, int action) {}
    virtual void on_mouse_move(int x, int y) {}
    virtual void on_mouse_wheel(int pos) {}

private:
    // Private Application data
    bool running = false;

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