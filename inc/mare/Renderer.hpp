#ifndef RENDERER
#define RENDERER

// Standard Library
#include <bitset>
// MARE
#include "mare/Buffer.hpp"

namespace mare
{

// Forward declarations for pointers
class Layer;
class Scene;
class SimpleMesh;
class Material;
class RenderState;
class Shader;
class Texture2D;

// The available cursors for the application to use
enum class CURSOR
{
    ARROW,
    HZ_RESIZE,
    HAND,
    CROSSHAIRS
};

// This info is set after Renderer creation but before the renderer executes
struct RendererInfo
{
    Scene *scene{};                              // Active Scene
    Layer *focus{};                                // Focused Layer
    const char* window_title{};                    // Window title
    int window_width{1280};                         // window width in pixels
    int window_height{720};                       // window height in pixels
    float window_aspect{16.0f / 9.0f};             // window aspect ratio
    double current_time{};                         // elapsed time in seconds
    int samples{};                                 // antialiasing samples
    bool wireframe{false};                         // render in wireframe mode?
    bool fullscreen{false};                        // render in fullscreen mode?
    bool vsync{false};                             // render in double buffered vsync mode?
    bool cursor{true};                             // render cursor?
    std::bitset<4> debug_mode{};                   // 0000 == off, 0001 == high, 0010 == med, 0100 == low, 1000 == notification
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
    bool APOSTROPHE_PRESSED{false};
    bool COMMA_PRESSED{false};
    bool MINUS_PRESSED{false};
    bool PERIOD_PRESSED{false};
    bool FORWARD_SLASH_PRESSED{false};
    bool ZERO_PRESSED{false};
    bool ONE_PRESSED{false};
    bool TWO_PRESSED{false};
    bool THREE_PRESSED{false};
    bool FOUR_PRESSED{false};
    bool FIVE_PRESSED{false};
    bool SIX_PRESSED{false};
    bool SEVEN_PRESSED{false};
    bool EIGHT_PRESSED{false};
    bool NINE_PRESSED{false};
    bool SEMICOLON_PRESSED{false};
    bool EQUAL_PRESSED{false};
    bool A_PRESSED{false};
    bool B_PRESSED{false};
    bool C_PRESSED{false};
    bool D_PRESSED{false};
    bool E_PRESSED{false};
    bool F_PRESSED{false};
    bool G_PRESSED{false};
    bool H_PRESSED{false};
    bool I_PRESSED{false};
    bool J_PRESSED{false};
    bool K_PRESSED{false};
    bool L_PRESSED{false};
    bool M_PRESSED{false};
    bool N_PRESSED{false};
    bool O_PRESSED{false};
    bool P_PRESSED{false};
    bool Q_PRESSED{false};
    bool R_PRESSED{false};
    bool S_PRESSED{false};
    bool T_PRESSED{false};
    bool U_PRESSED{false};
    bool V_PRESSED{false};
    bool W_PRESSED{false};
    bool X_PRESSED{false};
    bool Y_PRESSED{false};
    bool Z_PRESSED{false};
    bool LEFT_BRACKET_PRESSED{false};
    bool RIGHT_BRACKET_PRESSED{false};
    bool GRAVE_ACCENT_PRESSED{false};
    bool ESCAPE_PRESSED{false};
    bool ENTER_PRESSED{false};
    bool TAB_PRESSED{false};
    bool BACKSPACE_PRESSED{false};
    bool INSERT_PRESSED{false};
    bool DELETE_PRESSED{false};
    bool RIGHT_PRESSED{false};
    bool LEFT_PRESSED{false};
    bool UP_PRESSED{false};
    bool DOWN_PRESSED{false};
    bool PAGE_UP_PRESSED{false};
    bool PAGE_DOWN_PRESSED{false};
    bool HOME_PRESSED{false};
    bool END_PRESSED{false};
    bool CAPS_LOCK_PRESSED{false};
    bool SCROLL_LOCK_PRESSED{false};
    bool NUM_LOCK_PRESSED{false};
    bool PRINT_SCREEN_PRESSED{false};
    bool PAUSE_PRESSED{false};
    bool F1_PRESSED{false};
    bool F2_PRESSED{false};
    bool F3_PRESSED{false};
    bool F4_PRESSED{false};
    bool F5_PRESSED{false};
    bool F6_PRESSED{false};
    bool F7_PRESSED{false};
    bool F8_PRESSED{false};
    bool F9_PRESSED{false};
    bool F10_PRESSED{false};
    bool F11_PRESSED{false};
    bool F12_PRESSED{false};
    bool KEY_PAD_0_PRESSED{false};
    bool KEY_PAD_1_PRESSED{false};
    bool KEY_PAD_2_PRESSED{false};
    bool KEY_PAD_3_PRESSED{false};
    bool KEY_PAD_4_PRESSED{false};
    bool KEY_PAD_5_PRESSED{false};
    bool KEY_PAD_6_PRESSED{false};
    bool KEY_PAD_7_PRESSED{false};
    bool KEY_PAD_8_PRESSED{false};
    bool KEY_PAD_9_PRESSED{false};
    bool KEY_PAD_DECIMAL_PRESSED{false};
    bool KEY_PAD_DIVIDE_PRESSED{false};
    bool KEY_PAD_MULTIPLY_PRESSED{false};
    bool KEY_PAD_SUBTRACT_PRESSED{false};
    bool KEY_PAD_ADD_PRESSED{false};
    bool KEY_PAD_ENTER_PRESSED{false};
    bool KEY_PAD_EQUAL_PRESSED{false};
    bool LEFT_SHIFT_PRESSED{false};
    bool LEFT_CONTROL_PRESSED{false};
    bool LEFT_ALT_PRESSED{false};
    bool LEFT_SUPER_PRESSED{false};
    bool RIGHT_SHIFT_PRESSED{false};
    bool RIGHT_CONTROL_PRESSED{false};
    bool RIGHT_ALT_PRESSED{false};
    bool RIGHT_SUPER_PRESSED{false};
    bool MENU_PRESSED{false};
};

class Renderer
{
public:
    virtual ~Renderer() {}

    // Start and end the program
    static void run();
    virtual void start_process() = 0;
    static void end_process();

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

    // Buffers
    virtual Buffer<float> *GenFloatBuffer(unsigned int count) = 0;
    virtual Buffer<int> *GenIntBuffer(unsigned int count) = 0;
    virtual Buffer<unsigned int> *GenIndexBuffer(unsigned int count) = 0;
    virtual Buffer<bool> *GenBoolBuffer(unsigned int count) = 0;
    virtual Buffer<glm::vec2> *GenVec2Buffer(unsigned int count) = 0;
    virtual Buffer<glm::vec3> *GenVec3Buffer(unsigned int count) = 0;
    virtual Buffer<glm::vec4> *GenVec4Buffer(unsigned int count) = 0;
    virtual Buffer<glm::mat2> *GenMat2Buffer(unsigned int count) = 0;
    virtual Buffer<glm::mat3> *GenMat3Buffer(unsigned int count) = 0;
    virtual Buffer<glm::mat4> *GenMat4Buffer(unsigned int count) = 0;

    // Textures
    virtual Texture2D *GenTexture2D(const char *image_filepath) = 0;

    // Render States
    virtual RenderState *GenRenderState() = 0;

    // Shaders
    virtual Shader *GenShader(const char *directory) = 0;

    // Rendering Simple Meshes with no Composite or Instanced Meshes (Composite and Instanced Meshes are rendered by themselves)
    virtual void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material) = 0;
    virtual void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material, glm::mat4 parent_model) = 0;
    virtual void render_simple_mesh(Layer *layer, SimpleMesh *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;

    static Renderer* API;

protected:
    // static variables for renderer accessable from anywhere
    static RendererInfo info;
    static RendererInput input;
    static bool running;
};
} // namespace mare

#endif