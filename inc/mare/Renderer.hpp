#ifndef RENDERER
#define RENDERER

// Standard Library
#include <bitset>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <iostream>

// MARE
#include "mare/Mare.hpp"
#include "mare/Buffers.hpp"
#include "mare/GL/GLBuffers.hpp"
#include "mare/Shader.hpp"

namespace mare
{
// Forward Declarations
class SimpleMesh;
class Layer;
class Camera;
class Scene;

// The available cursors for the application to use
enum class CURSOR
{
    DISABLED,
    ENABLED,
    ARROW,
    HZ_RESIZE,
    HAND,
    CROSSHAIRS
};

enum class RendererAPI
{
    NONE = 0,
    OpenGL_4_5,
    DirectX_12, // Not yet supported
    Vulkan      // Not yet supported
};

// This info is set after Renderer creation but before the renderer executes
struct RendererInfo
{
    Scene *scene{};                    // Active Scene
    Layer *focus{};                    // Focused Layer
    const char *window_title{};        // Window title
    int window_width{1280};            // window width in pixels
    int window_height{720};            // window height in pixels
    float window_aspect{16.0f / 9.0f}; // window aspect ratio
    double current_time{};             // elapsed time in seconds
    int samples{};                     // antialiasing samples
    bool wireframe{false};             // render in wireframe mode?
    bool fullscreen{false};            // render in fullscreen mode?
    bool vsync{false};                 // render in double buffered vsync mode?
    bool cursor{true};                 // render cursor?
    std::bitset<4> debug_mode{};       // 0000 == off, 0001 == high, 0010 == med, 0100 == low, 1000 == notification
    RendererAPI API{RendererAPI::NONE};
};

// This input is passed by reference to the callbacks which decide what to do with it
struct RendererInput
{
    std::bitset<3> mouse_button{}; // 001 == left, 010 == right, 100 == middle
    short mouse_scroll{};          // 0 == no scroll, 1 == scroll up, -1 = scroll down
    glm::ivec2 mouse_pos{};        // window coordinates of mouse position (in pixels from top left corner)
    glm::ivec2 mouse_vel{};        // window coordinates of mouse velocity (in pixels from top left corner)
    // Key presses
    bool SPACE_PRESSED{false};
    bool SPACE_JUST_PRESSED{false};
    bool APOSTROPHE_PRESSED{false};
    bool APOSTROPHE_JUST_PRESSED{false};
    bool COMMA_PRESSED{false};
    bool COMMA_JUST_PRESSED{false};
    bool MINUS_PRESSED{false};
    bool MINUS_JUST_PRESSED{false};
    bool PERIOD_PRESSED{false};
    bool PERIOD_JUST_PRESSED{false};
    bool FORWARD_SLASH_PRESSED{false};
    bool FORWARD_SLASH_JUST_PRESSED{false};
    bool ZERO_PRESSED{false};
    bool ZERO_JUST_PRESSED{false};
    bool ONE_PRESSED{false};
    bool ONE_JUST_PRESSED{false};
    bool TWO_PRESSED{false};
    bool TWO_JUST_PRESSED{false};
    bool THREE_PRESSED{false};
    bool THREE_JUST_PRESSED{false};
    bool FOUR_PRESSED{false};
    bool FOUR_JUST_PRESSED{false};
    bool FIVE_PRESSED{false};
    bool FIVE_JUST_PRESSED{false};
    bool SIX_PRESSED{false};
    bool SIX_JUST_PRESSED{false};
    bool SEVEN_PRESSED{false};
    bool SEVEN_JUST_PRESSED{false};
    bool EIGHT_PRESSED{false};
    bool EIGHT_JUST_PRESSED{false};
    bool NINE_PRESSED{false};
    bool NINE_JUST_PRESSED{false};
    bool SEMICOLON_PRESSED{false};
    bool SEMICOLON_JUST_PRESSED{false};
    bool EQUAL_PRESSED{false};
    bool EQUAL_JUST_PRESSED{false};
    bool A_PRESSED{false};
    bool A_JUST_PRESSED{false};
    bool B_PRESSED{false};
    bool B_JUST_PRESSED{false};
    bool C_PRESSED{false};
    bool C_JUST_PRESSED{false};
    bool D_PRESSED{false};
    bool D_JUST_PRESSED{false};
    bool E_PRESSED{false};
    bool E_JUST_PRESSED{false};
    bool F_PRESSED{false};
    bool F_JUST_PRESSED{false};
    bool G_PRESSED{false};
    bool G_JUST_PRESSED{false};
    bool H_PRESSED{false};
    bool H_JUST_PRESSED{false};
    bool I_PRESSED{false};
    bool I_JUST_PRESSED{false};
    bool J_PRESSED{false};
    bool J_JUST_PRESSED{false};
    bool K_PRESSED{false};
    bool K_JUST_PRESSED{false};
    bool L_PRESSED{false};
    bool L_JUST_PRESSED{false};
    bool M_PRESSED{false};
    bool M_JUST_PRESSED{false};
    bool N_PRESSED{false};
    bool N_JUST_PRESSED{false};
    bool O_PRESSED{false};
    bool O_JUST_PRESSED{false};
    bool P_PRESSED{false};
    bool P_JUST_PRESSED{false};
    bool Q_PRESSED{false};
    bool Q_JUST_PRESSED{false};
    bool R_PRESSED{false};
    bool R_JUST_PRESSED{false};
    bool S_PRESSED{false};
    bool S_JUST_PRESSED{false};
    bool T_PRESSED{false};
    bool T_JUST_PRESSED{false};
    bool U_PRESSED{false};
    bool U_JUST_PRESSED{false};
    bool V_PRESSED{false};
    bool V_JUST_PRESSED{false};
    bool W_PRESSED{false};
    bool W_JUST_PRESSED{false};
    bool X_PRESSED{false};
    bool X_JUST_PRESSED{false};
    bool Y_PRESSED{false};
    bool Y_JUST_PRESSED{false};
    bool Z_PRESSED{false};
    bool Z_JUST_PRESSED{false};
    bool LEFT_BRACKET_PRESSED{false};
    bool LEFT_BRACKET_JUST_PRESSED{false};
    bool RIGHT_BRACKET_PRESSED{false};
    bool RIGHT_BRACKET_JUST_PRESSED{false};
    bool GRAVE_ACCENT_PRESSED{false};
    bool GRAVE_ACCENT_JUST_PRESSED{false};
    bool ESCAPE_PRESSED{false};
    bool ESCAPE_JUST_PRESSED{false};
    bool ENTER_PRESSED{false};
    bool ENTER_JUST_PRESSED{false};
    bool TAB_PRESSED{false};
    bool TAB_JUST_PRESSED{false};
    bool BACKSPACE_PRESSED{false};
    bool BACKSPACE_JUST_PRESSED{false};
    bool INSERT_PRESSED{false};
    bool INSERT_JUST_PRESSED{false};
    bool DELETE_PRESSED{false};
    bool DELETE_JUST_PRESSED{false};
    bool RIGHT_PRESSED{false};
    bool RIGHT_JUST_PRESSED{false};
    bool LEFT_PRESSED{false};
    bool LEFT_JUST_PRESSED{false};
    bool UP_PRESSED{false};
    bool UP_JUST_PRESSED{false};
    bool DOWN_PRESSED{false};
    bool DOWN_JUST_PRESSED{false};
    bool PAGE_UP_PRESSED{false};
    bool PAGE_UP_JUST_PRESSED{false};
    bool PAGE_DOWN_PRESSED{false};
    bool PAGE_DOWN_JUST_PRESSED{false};
    bool HOME_PRESSED{false};
    bool HOME_JUST_PRESSED{false};
    bool END_PRESSED{false};
    bool END_JUST_PRESSED{false};
    bool CAPS_LOCK_PRESSED{false};
    bool CAPS_LOCK_JUST_PRESSED{false};
    bool SCROLL_LOCK_PRESSED{false};
    bool SCROLL_LOCK_JUST_PRESSED{false};
    bool NUM_LOCK_PRESSED{false};
    bool NUM_LOCK_JUST_PRESSED{false};
    bool PRINT_SCREEN_PRESSED{false};
    bool PRINT_SCREEN_JUST_PRESSED{false};
    bool PAUSE_PRESSED{false};
    bool PAUSE_JUST_PRESSED{false};
    bool F1_PRESSED{false};
    bool F1_JUST_PRESSED{false};
    bool F2_PRESSED{false};
    bool F2_JUST_PRESSED{false};
    bool F3_PRESSED{false};
    bool F3_JUST_PRESSED{false};
    bool F4_PRESSED{false};
    bool F4_JUST_PRESSED{false};
    bool F5_PRESSED{false};
    bool F5_JUST_PRESSED{false};
    bool F6_PRESSED{false};
    bool F6_JUST_PRESSED{false};
    bool F7_PRESSED{false};
    bool F7_JUST_PRESSED{false};
    bool F8_PRESSED{false};
    bool F8_JUST_PRESSED{false};
    bool F9_PRESSED{false};
    bool F9_JUST_PRESSED{false};
    bool F10_PRESSED{false};
    bool F10_JUST_PRESSED{false};
    bool F11_PRESSED{false};
    bool F11_JUST_PRESSED{false};
    bool F12_PRESSED{false};
    bool F12_JUST_PRESSED{false};
    bool KEY_PAD_0_PRESSED{false};
    bool KEY_PAD_0_JUST_PRESSED{false};
    bool KEY_PAD_1_PRESSED{false};
    bool KEY_PAD_1_JUST_PRESSED{false};
    bool KEY_PAD_2_PRESSED{false};
    bool KEY_PAD_2_JUST_PRESSED{false};
    bool KEY_PAD_3_PRESSED{false};
    bool KEY_PAD_3_JUST_PRESSED{false};
    bool KEY_PAD_4_PRESSED{false};
    bool KEY_PAD_4_JUST_PRESSED{false};
    bool KEY_PAD_5_PRESSED{false};
    bool KEY_PAD_5_JUST_PRESSED{false};
    bool KEY_PAD_6_PRESSED{false};
    bool KEY_PAD_6_JUST_PRESSED{false};
    bool KEY_PAD_7_PRESSED{false};
    bool KEY_PAD_7_JUST_PRESSED{false};
    bool KEY_PAD_8_PRESSED{false};
    bool KEY_PAD_8_JUST_PRESSED{false};
    bool KEY_PAD_9_PRESSED{false};
    bool KEY_PAD_9_JUST_PRESSED{false};
    bool KEY_PAD_DECIMAL_PRESSED{false};
    bool KEY_PAD_DECIMAL_JUST_PRESSED{false};
    bool KEY_PAD_DIVIDE_PRESSED{false};
    bool KEY_PAD_DIVIDE_JUST_PRESSED{false};
    bool KEY_PAD_MULTIPLY_PRESSED{false};
    bool KEY_PAD_MULTIPLY_JUST_PRESSED{false};
    bool KEY_PAD_SUBTRACT_PRESSED{false};
    bool KEY_PAD_SUBTRACT_JUST_PRESSED{false};
    bool KEY_PAD_ADD_PRESSED{false};
    bool KEY_PAD_ADD_JUST_PRESSED{false};
    bool KEY_PAD_ENTER_PRESSED{false};
    bool KEY_PAD_ENTER_JUST_PRESSED{false};
    bool KEY_PAD_EQUAL_PRESSED{false};
    bool KEY_PAD_EQUAL_JUST_PRESSED{false};
    bool LEFT_SHIFT_PRESSED{false};
    bool LEFT_SHIFT_JUST_PRESSED{false};
    bool LEFT_CONTROL_PRESSED{false};
    bool LEFT_CONTROL_JUST_PRESSED{false};
    bool LEFT_ALT_PRESSED{false};
    bool LEFT_ALT_JUST_PRESSED{false};
    bool LEFT_SUPER_PRESSED{false};
    bool LEFT_SUPER_JUST_PRESSED{false};
    bool RIGHT_SHIFT_PRESSED{false};
    bool RIGHT_SHIFT_JUST_PRESSED{false};
    bool RIGHT_CONTROL_PRESSED{false};
    bool RIGHT_CONTROL_JUST_PRESSED{false};
    bool RIGHT_ALT_PRESSED{false};
    bool RIGHT_ALT_JUST_PRESSED{false};
    bool RIGHT_SUPER_PRESSED{false};
    bool RIGHT_SUPER_JUST_PRESSED{false};
    bool MENU_PRESSED{false};
    bool MENU_JUST_PRESSED{false};
};

class Renderer
{
public:
    virtual ~Renderer() {}

    // Start and end the program
    virtual void run() = 0;
    virtual void init_renderer() = 0;
    virtual void start_renderer() = 0;
    virtual void init_info() = 0;
    virtual void startup() {}
    virtual void shutdown() {}
    static void end_renderer();

    // Renderer information and input
    static RendererInfo &get_info();
    static RendererInput &get_input();

    // Renderer Commands
    virtual void set_window_title(const char *title) = 0;
    virtual void set_cursor(CURSOR type) = 0;
    virtual void clear_color_buffer(glm::vec4 color) = 0;
    virtual void clear_depth_buffer() = 0;
    virtual void resize_window(int width, int height) = 0;
    virtual void wireframe_mode(bool wireframe) = 0;
    virtual void enable_depth_testing(bool enable) = 0;
    virtual void enable_face_culling(bool enable) = 0;
    virtual void enable_blending(bool enable) = 0;
    virtual glm::vec3 raycast(Camera *camera) = 0;
    virtual glm::vec3 raycast(Camera *camera, glm::ivec2 screen_coords) = 0;
    virtual void set_framebuffer(Framebuffer* framebuffer) = 0;

    // Scenes
    static void load_scene(Scene *scene);
    static void load_scene(std::string name);

    // Buffers
    template <typename T>
    Scoped<Buffer<T>> GenBuffer(T *data, size_t size_in_bytes, BufferType buffer_type = BufferType::STATIC)
    {
        switch (info.API)
        {
        case RendererAPI::OpenGL_4_5:
            return std::make_unique<GLBuffer<T>>(data, size_in_bytes, buffer_type);
            break;
        default:
            return nullptr;
        }
    }

    // Textures
    virtual Scoped<Texture2DBuffer> GenTexture2DBuffer(const char *image_filepath) = 0;
    virtual Scoped<Texture2DBuffer> GenTexture2DBuffer(TextureType type, int width, int height) = 0;

    // Framebuffers
    virtual Scoped<Framebuffer> GenFramebuffer(int width, int height) = 0;

    // Shaders
    virtual Scoped<Shader> GenShader(const char *directory) = 0;

    // Rendering Simple Meshes with no Composite or Instanced Meshes (Composite and Instanced Meshes are rendered by themselves)
    virtual void render_simple_mesh(Camera *camera, SimpleMesh *mesh, Material *material) = 0;
    virtual void render_simple_mesh(Camera *camera, SimpleMesh *mesh, Material *material, glm::mat4 &parent_model) = 0;
    virtual void render_simple_mesh(Camera *camera, SimpleMesh *mesh, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
    virtual void bind_mesh_render_state(SimpleMesh* mesh, Material* material) = 0;
    virtual void destroy_mesh_render_states(SimpleMesh* mesh) = 0;
    virtual void push_mesh_geometry_buffer(SimpleMesh* mesh, Scoped<Buffer<float>> geometry_buffer) = 0;
    virtual void set_mesh_index_buffer(SimpleMesh* mesh, Scoped<Buffer<uint32_t>> index_buffer) = 0;

    // Compute Programs
    virtual void dispatch_compute(uint32_t x, uint32_t y, uint32_t z) = 0;

    // Create scoped or referenced assets
    template <typename T, typename... Args>
    Scoped<T> GenScoped(Args... args)
    {
        return std::make_unique<T>(args...);
    }
    template <typename T, typename... Args>
    Referenced<T> GenNamedRef(std::string name, Args... args)
    {
        asset_map.insert_or_assign(name, std::make_shared<T>(args...));
        return std::dynamic_pointer_cast<T>(asset_map[name]);
    }
    template <typename T, typename... Args>
    Referenced<T> GenRef(Args... args)
    {
        return std::make_shared<T>(args...);
    }
    template <typename T>
    Referenced<T> GetRefByName(std::string name)
    {
        if (asset_map.find(name) != asset_map.end())
        {
            return std::dynamic_pointer_cast<T>(asset_map[name]);
        }
        return nullptr;
    }
    void DeleteRef(std::string name)
    {
        asset_map.erase(name);
    }

    static Renderer *API;

protected:
    // static variables for renderer accessable from anywhere
    static RendererInfo info;
    static RendererInput input;
    static bool running;

    // Referenced Assests
    std::unordered_map<std::string, Referenced<Asset>> asset_map;
};

template <typename T>
int launch()
{
    static_assert(std::is_base_of<Renderer, T>::value, "Type must be a renderer.");
    try
    {
        std::unique_ptr<T> renderer = std::make_unique<T>();
        Renderer::API = renderer.get();
        Renderer::API->run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}

} // namespace mare

#endif