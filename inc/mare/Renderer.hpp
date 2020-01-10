#ifndef RENDERER
#define RENDERER

// Standard Library
#include <bitset>
// External Libraries
#include "glm.hpp"

namespace mare
{

class Application; // forward declaration

enum class RendererName
{
    OpenGL,
    DirectX
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
    RendererName name {};
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
};

class Renderer
{
public:
    virtual ~Renderer() {}
    virtual void renderer_process(Application *app_pointer) = 0;
    virtual void set_window_title(const char *title) = 0;
    virtual void set_cursor(CURSOR type) = 0;

    static inline RendererInfo &get_info() { return info; }
    static inline RendererInput &get_input() { return input; }
    static inline void end_process() { running = false; }

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
    static bool running;
};
} // namespace mare

#endif