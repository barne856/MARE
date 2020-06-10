#ifndef RENDERER
#define RENDERER

// Standard Library
#include <bitset>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>

// MARE
#include "Buffers.hpp"
#include "GL/GLBuffers.hpp"
#include "Mare.hpp"
#include "Shader.hpp"

namespace mare {
// Forward Declarations
class SimpleMesh;
class UIElement;
class Layer;
class Camera;
class Scene;

/**
 * @brief The available cursors for the application to use.
 */
enum class CursorType {
  DISABLED,  /**< No cursor is displayed.*/
  ENABLED,   /**< Enable the cursor.*/
  ARROW,     /**< The default arrow cursor.*/
  HZ_RESIZE, /**< Horizontal resize cursor.*/
  HAND,      /**< Hand cursor.*/
  CROSSHAIRS /**< Crosshairs cursor.*/
};

/**
 * @brief The selected Rendering API.
 */
enum class RendererAPI {
  NONE = 0,   /**< No API selected.*/
  OpenGL_4_5, /**< OpenGL 4.5 API.*/
  DirectX_12, /**< DirectX 12 API (Not supported yet).*/
  Vulkan      /**< Vulkan API (Not supported yet).*/
};

/**
 * @brief The properties of the implimented Rendering API.
 * @details These properties are set after the Renderer creation but before the
 * renderer begins the render loop.
 */
struct RendererInfo {
  Scene *scene{nullptr};               /**< Active Scene*/
  UIElement *focused_element{nullptr}; /**< Focused UIElement*/
  const char *window_title{nullptr};   /**< Window title*/
  int window_width{1280};              /**< Window width in pixels*/
  int window_height{720};              /**< Window height in pixels*/
  float window_aspect{16.0f / 9.0f};   /**< Window aspect ratio*/
  double current_time{0.0f};           /**< Elapsed time in seconds*/
  int samples{0};                      /**< Antialiasing samples*/
  bool wireframe{false};               /**< Render in wireframe mode?*/
  bool fullscreen{false};              /**< Render in fullscreen mode?*/
  bool vsync{false}; /**< Render in double buffered vsync mode?*/
  bool cursor{true}; /**< Render cursor?*/
  std::bitset<4> debug_mode{
      1}; /**< 0000 == off, 0001 == high, 0010 == med, 0100
            == low, 1000 == notification*/
  RendererAPI API{RendererAPI::NONE}; /**< The Rendering API.*/
};

// This input is passed by reference to the callbacks which decide what to do
// with it
/**
 * @brief RendererInput contains the user input and is passed by reference to
 * the callbacks which decide what to do.
 * @details Keyboard and mouse input as well as mouse position and velocity.
 */
struct RendererInput {
  short mouse_scroll{}; /**< 0 == no scroll, 1 == scroll up, -1 = scroll down*/
  glm::ivec2 mouse_pos{}; /**< window coordinates of mouse position (in pixels
                             from top left corner)*/
  glm::ivec2 mouse_vel{}; /**< window coordinates of mouse velocity (in
                             pixels/frame from top left corner)*/
  // Mouse clicks
  bool LEFT_MOUSE_PRESSED{false};
  bool MIDDLE_MOUSE_PRESSED{false};
  bool RIGHT_MOUSE_PRESSED{false};
  bool LEFT_MOUSE_JUST_PRESSED{false};
  bool MIDDLE_MOUSE_JUST_PRESSED{false};
  bool RIGHT_MOUSE_JUST_PRESSED{false};
  bool LEFT_MOUSE_JUST_RELEASED{false};
  bool MIDDLE_MOUSE_JUST_RELEASED{false};
  bool RIGHT_MOUSE_JUST_RELEASED{false};
  // Key presses
  bool SPACE_PRESSED{false};
  bool SPACE_JUST_PRESSED{false};
  bool SPACE_JUST_RELEASED{false};
  bool APOSTROPHE_PRESSED{false};
  bool APOSTROPHE_JUST_PRESSED{false};
  bool APOSTROPHE_JUST_RELEASED{false};
  bool COMMA_PRESSED{false};
  bool COMMA_JUST_PRESSED{false};
  bool COMMA_JUST_RELEASED{false};
  bool MINUS_PRESSED{false};
  bool MINUS_JUST_PRESSED{false};
  bool MINUS_JUST_RELEASED{false};
  bool PERIOD_PRESSED{false};
  bool PERIOD_JUST_PRESSED{false};
  bool PERIOD_JUST_RELEASED{false};
  bool FORWARD_SLASH_PRESSED{false};
  bool FORWARD_SLASH_JUST_PRESSED{false};
  bool FORWARD_SLASH_JUST_RELEASED{false};
  bool ZERO_PRESSED{false};
  bool ZERO_JUST_PRESSED{false};
  bool ZERO_JUST_RELEASED{false};
  bool ONE_PRESSED{false};
  bool ONE_JUST_PRESSED{false};
  bool ONE_JUST_RELEASED{false};
  bool TWO_PRESSED{false};
  bool TWO_JUST_PRESSED{false};
  bool TWO_JUST_RELEASED{false};
  bool THREE_PRESSED{false};
  bool THREE_JUST_PRESSED{false};
  bool THREE_JUST_RELEASED{false};
  bool FOUR_PRESSED{false};
  bool FOUR_JUST_PRESSED{false};
  bool FOUR_JUST_RELEASED{false};
  bool FIVE_PRESSED{false};
  bool FIVE_JUST_PRESSED{false};
  bool FIVE_JUST_RELEASED{false};
  bool SIX_PRESSED{false};
  bool SIX_JUST_PRESSED{false};
  bool SIX_JUST_RELEASED{false};
  bool SEVEN_PRESSED{false};
  bool SEVEN_JUST_PRESSED{false};
  bool SEVEN_JUST_RELEASED{false};
  bool EIGHT_PRESSED{false};
  bool EIGHT_JUST_PRESSED{false};
  bool EIGHT_JUST_RELEASED{false};
  bool NINE_PRESSED{false};
  bool NINE_JUST_PRESSED{false};
  bool NINE_JUST_RELEASED{false};
  bool SEMICOLON_PRESSED{false};
  bool SEMICOLON_JUST_PRESSED{false};
  bool SEMICOLON_JUST_RELEASED{false};
  bool EQUAL_PRESSED{false};
  bool EQUAL_JUST_PRESSED{false};
  bool EQUAL_JUST_RELEASED{false};
  bool A_PRESSED{false};
  bool A_JUST_PRESSED{false};
  bool A_JUST_RELEASED{false};
  bool B_PRESSED{false};
  bool B_JUST_PRESSED{false};
  bool B_JUST_RELEASED{false};
  bool C_PRESSED{false};
  bool C_JUST_PRESSED{false};
  bool C_JUST_RELEASED{false};
  bool D_PRESSED{false};
  bool D_JUST_PRESSED{false};
  bool D_JUST_RELEASED{false};
  bool E_PRESSED{false};
  bool E_JUST_PRESSED{false};
  bool E_JUST_RELEASED{false};
  bool F_PRESSED{false};
  bool F_JUST_PRESSED{false};
  bool F_JUST_RELEASED{false};
  bool G_PRESSED{false};
  bool G_JUST_PRESSED{false};
  bool G_JUST_RELEASED{false};
  bool H_PRESSED{false};
  bool H_JUST_PRESSED{false};
  bool H_JUST_RELEASED{false};
  bool I_PRESSED{false};
  bool I_JUST_PRESSED{false};
  bool I_JUST_RELEASED{false};
  bool J_PRESSED{false};
  bool J_JUST_PRESSED{false};
  bool J_JUST_RELEASED{false};
  bool K_PRESSED{false};
  bool K_JUST_PRESSED{false};
  bool K_JUST_RELEASED{false};
  bool L_PRESSED{false};
  bool L_JUST_PRESSED{false};
  bool L_JUST_RELEASED{false};
  bool M_PRESSED{false};
  bool M_JUST_PRESSED{false};
  bool M_JUST_RELEASED{false};
  bool N_PRESSED{false};
  bool N_JUST_PRESSED{false};
  bool N_JUST_RELEASED{false};
  bool O_PRESSED{false};
  bool O_JUST_PRESSED{false};
  bool O_JUST_RELEASED{false};
  bool P_PRESSED{false};
  bool P_JUST_PRESSED{false};
  bool P_JUST_RELEASED{false};
  bool Q_PRESSED{false};
  bool Q_JUST_PRESSED{false};
  bool Q_JUST_RELEASED{false};
  bool R_PRESSED{false};
  bool R_JUST_PRESSED{false};
  bool R_JUST_RELEASED{false};
  bool S_PRESSED{false};
  bool S_JUST_PRESSED{false};
  bool S_JUST_RELEASED{false};
  bool T_PRESSED{false};
  bool T_JUST_PRESSED{false};
  bool T_JUST_RELEASED{false};
  bool U_PRESSED{false};
  bool U_JUST_PRESSED{false};
  bool U_JUST_RELEASED{false};
  bool V_PRESSED{false};
  bool V_JUST_PRESSED{false};
  bool V_JUST_RELEASED{false};
  bool W_PRESSED{false};
  bool W_JUST_PRESSED{false};
  bool W_JUST_RELEASED{false};
  bool X_PRESSED{false};
  bool X_JUST_PRESSED{false};
  bool X_JUST_RELEASED{false};
  bool Y_PRESSED{false};
  bool Y_JUST_PRESSED{false};
  bool Y_JUST_RELEASED{false};
  bool Z_PRESSED{false};
  bool Z_JUST_PRESSED{false};
  bool Z_JUST_RELEASED{false};
  bool LEFT_BRACKET_PRESSED{false};
  bool LEFT_BRACKET_JUST_PRESSED{false};
  bool LEFT_BRACKET_JUST_RELEASED{false};
  bool RIGHT_BRACKET_PRESSED{false};
  bool RIGHT_BRACKET_JUST_PRESSED{false};
  bool RIGHT_BRACKET_JUST_RELEASED{false};
  bool GRAVE_ACCENT_PRESSED{false};
  bool GRAVE_ACCENT_JUST_PRESSED{false};
  bool GRAVE_ACCENT_JUST_RELEASED{false};
  bool ESCAPE_PRESSED{false};
  bool ESCAPE_JUST_PRESSED{false};
  bool ESCAPE_JUST_RELEASED{false};
  bool ENTER_PRESSED{false};
  bool ENTER_JUST_PRESSED{false};
  bool ENTER_JUST_RELEASED{false};
  bool TAB_PRESSED{false};
  bool TAB_JUST_PRESSED{false};
  bool TAB_JUST_RELEASED{false};
  bool BACKSPACE_PRESSED{false};
  bool BACKSPACE_JUST_PRESSED{false};
  bool BACKSPACE_JUST_RELEASED{false};
  bool INSERT_PRESSED{false};
  bool INSERT_JUST_PRESSED{false};
  bool INSERT_JUST_RELEASED{false};
  bool DELETE_PRESSED{false};
  bool DELETE_JUST_PRESSED{false};
  bool DELETE_JUST_RELEASED{false};
  bool RIGHT_PRESSED{false};
  bool RIGHT_JUST_PRESSED{false};
  bool RIGHT_JUST_RELEASED{false};
  bool LEFT_PRESSED{false};
  bool LEFT_JUST_PRESSED{false};
  bool LEFT_JUST_RELEASED{false};
  bool UP_PRESSED{false};
  bool UP_JUST_PRESSED{false};
  bool UP_JUST_RELEASED{false};
  bool DOWN_PRESSED{false};
  bool DOWN_JUST_PRESSED{false};
  bool DOWN_JUST_RELEASED{false};
  bool PAGE_UP_PRESSED{false};
  bool PAGE_UP_JUST_PRESSED{false};
  bool PAGE_UP_JUST_RELEASED{false};
  bool PAGE_DOWN_PRESSED{false};
  bool PAGE_DOWN_JUST_PRESSED{false};
  bool PAGE_DOWN_JUST_RELEASED{false};
  bool HOME_PRESSED{false};
  bool HOME_JUST_PRESSED{false};
  bool HOME_JUST_RELEASED{false};
  bool END_PRESSED{false};
  bool END_JUST_PRESSED{false};
  bool END_JUST_RELEASED{false};
  bool CAPS_LOCK_PRESSED{false};
  bool CAPS_LOCK_JUST_PRESSED{false};
  bool CAPS_LOCK_JUST_RELEASED{false};
  bool SCROLL_LOCK_PRESSED{false};
  bool SCROLL_LOCK_JUST_PRESSED{false};
  bool SCROLL_LOCK_JUST_RELEASED{false};
  bool NUM_LOCK_PRESSED{false};
  bool NUM_LOCK_JUST_PRESSED{false};
  bool NUM_LOCK_JUST_RELEASED{false};
  bool PRINT_SCREEN_PRESSED{false};
  bool PRINT_SCREEN_JUST_PRESSED{false};
  bool PRINT_SCREEN_JUST_RELEASED{false};
  bool PAUSE_PRESSED{false};
  bool PAUSE_JUST_PRESSED{false};
  bool PAUSE_JUST_RELEASED{false};
  bool F1_PRESSED{false};
  bool F1_JUST_PRESSED{false};
  bool F1_JUST_RELEASED{false};
  bool F2_PRESSED{false};
  bool F2_JUST_PRESSED{false};
  bool F2_JUST_RELEASED{false};
  bool F3_PRESSED{false};
  bool F3_JUST_PRESSED{false};
  bool F3_JUST_RELEASED{false};
  bool F4_PRESSED{false};
  bool F4_JUST_PRESSED{false};
  bool F4_JUST_RELEASED{false};
  bool F5_PRESSED{false};
  bool F5_JUST_PRESSED{false};
  bool F5_JUST_RELEASED{false};
  bool F6_PRESSED{false};
  bool F6_JUST_PRESSED{false};
  bool F6_JUST_RELEASED{false};
  bool F7_PRESSED{false};
  bool F7_JUST_PRESSED{false};
  bool F7_JUST_RELEASED{false};
  bool F8_PRESSED{false};
  bool F8_JUST_PRESSED{false};
  bool F8_JUST_RELEASED{false};
  bool F9_PRESSED{false};
  bool F9_JUST_PRESSED{false};
  bool F9_JUST_RELEASED{false};
  bool F10_PRESSED{false};
  bool F10_JUST_PRESSED{false};
  bool F10_JUST_RELEASED{false};
  bool F11_PRESSED{false};
  bool F11_JUST_PRESSED{false};
  bool F11_JUST_RELEASED{false};
  bool F12_PRESSED{false};
  bool F12_JUST_PRESSED{false};
  bool F12_JUST_RELEASED{false};
  bool KEY_PAD_0_PRESSED{false};
  bool KEY_PAD_0_JUST_PRESSED{false};
  bool KEY_PAD_0_JUST_RELEASED{false};
  bool KEY_PAD_1_PRESSED{false};
  bool KEY_PAD_1_JUST_PRESSED{false};
  bool KEY_PAD_1_JUST_RELEASED{false};
  bool KEY_PAD_2_PRESSED{false};
  bool KEY_PAD_2_JUST_PRESSED{false};
  bool KEY_PAD_2_JUST_RELEASED{false};
  bool KEY_PAD_3_PRESSED{false};
  bool KEY_PAD_3_JUST_PRESSED{false};
  bool KEY_PAD_3_JUST_RELEASED{false};
  bool KEY_PAD_4_PRESSED{false};
  bool KEY_PAD_4_JUST_PRESSED{false};
  bool KEY_PAD_4_JUST_RELEASED{false};
  bool KEY_PAD_5_PRESSED{false};
  bool KEY_PAD_5_JUST_PRESSED{false};
  bool KEY_PAD_5_JUST_RELEASED{false};
  bool KEY_PAD_6_PRESSED{false};
  bool KEY_PAD_6_JUST_PRESSED{false};
  bool KEY_PAD_6_JUST_RELEASED{false};
  bool KEY_PAD_7_PRESSED{false};
  bool KEY_PAD_7_JUST_PRESSED{false};
  bool KEY_PAD_7_JUST_RELEASED{false};
  bool KEY_PAD_8_PRESSED{false};
  bool KEY_PAD_8_JUST_PRESSED{false};
  bool KEY_PAD_8_JUST_RELEASED{false};
  bool KEY_PAD_9_PRESSED{false};
  bool KEY_PAD_9_JUST_PRESSED{false};
  bool KEY_PAD_9_JUST_RELEASED{false};
  bool KEY_PAD_DECIMAL_PRESSED{false};
  bool KEY_PAD_DECIMAL_JUST_PRESSED{false};
  bool KEY_PAD_DECIMAL_JUST_RELEASED{false};
  bool KEY_PAD_DIVIDE_PRESSED{false};
  bool KEY_PAD_DIVIDE_JUST_PRESSED{false};
  bool KEY_PAD_DIVIDE_JUST_RELEASED{false};
  bool KEY_PAD_MULTIPLY_PRESSED{false};
  bool KEY_PAD_MULTIPLY_JUST_PRESSED{false};
  bool KEY_PAD_MULTIPLY_JUST_RELEASED{false};
  bool KEY_PAD_SUBTRACT_PRESSED{false};
  bool KEY_PAD_SUBTRACT_JUST_PRESSED{false};
  bool KEY_PAD_SUBTRACT_JUST_RELEASED{false};
  bool KEY_PAD_ADD_PRESSED{false};
  bool KEY_PAD_ADD_JUST_PRESSED{false};
  bool KEY_PAD_ADD_JUST_RELEASED{false};
  bool KEY_PAD_ENTER_PRESSED{false};
  bool KEY_PAD_ENTER_JUST_PRESSED{false};
  bool KEY_PAD_ENTER_JUST_RELEASED{false};
  bool KEY_PAD_EQUAL_PRESSED{false};
  bool KEY_PAD_EQUAL_JUST_PRESSED{false};
  bool KEY_PAD_EQUAL_JUST_RELEASED{false};
  bool LEFT_SHIFT_PRESSED{false};
  bool LEFT_SHIFT_JUST_PRESSED{false};
  bool LEFT_SHIFT_JUST_RELEASED{false};
  bool LEFT_CONTROL_PRESSED{false};
  bool LEFT_CONTROL_JUST_PRESSED{false};
  bool LEFT_CONTROL_JUST_RELEASED{false};
  bool LEFT_ALT_PRESSED{false};
  bool LEFT_ALT_JUST_PRESSED{false};
  bool LEFT_ALT_JUST_RELEASED{false};
  bool LEFT_SUPER_PRESSED{false};
  bool LEFT_SUPER_JUST_PRESSED{false};
  bool LEFT_SUPER_JUST_RELEASED{false};
  bool RIGHT_SHIFT_PRESSED{false};
  bool RIGHT_SHIFT_JUST_PRESSED{false};
  bool RIGHT_SHIFT_JUST_RELEASED{false};
  bool RIGHT_CONTROL_PRESSED{false};
  bool RIGHT_CONTROL_JUST_PRESSED{false};
  bool RIGHT_CONTROL_JUST_RELEASED{false};
  bool RIGHT_ALT_PRESSED{false};
  bool RIGHT_ALT_JUST_PRESSED{false};
  bool RIGHT_ALT_JUST_RELEASED{false};
  bool RIGHT_SUPER_PRESSED{false};
  bool RIGHT_SUPER_JUST_PRESSED{false};
  bool RIGHT_SUPER_JUST_RELEASED{false};
  bool MENU_PRESSED{false};
  bool MENU_JUST_PRESSED{false};
  bool MENU_JUST_RELEASED{false};
};

/**
 * @brief The abstract base class for a Rendering API implementation.
 * @details The user inherits from a Rendering API implementation and launches
 * it to start the program.
 * @see mare::launch<T>()
 */
class Renderer {
public:
  /**
   * @brief Virtual destructor of the Renderer object
   */
  virtual ~Renderer() {}

  /**
   * @brief This is the driver function of the renderer and is implemented by
   * the chosen Rendering API.
   * @details This function will initialize the Rendering API and start the
   * rendering loop.
   */
  virtual void run() = 0;
  /**
   * @brief This function is implemented by the Rendering API and will perform
   * any neccesary initialization and window creation.
   * @details Called by Renderer::run()
   */
  virtual void init_renderer() = 0;
  /**
   * @brief This function is implemented by the Rendering API and will start the
   * render loop.
   * @details Called by Renderer::run()
   */
  virtual void start_renderer() = 0;
  /**
   * @brief This function is implemented by the user's implementation of the
   * chosen Rendering API and its purpose is to set the Renderer's properties
   * for initialization.
   * @details Called by Renderer::run() before init_renderer()
   */
  virtual void init_info() = 0;
  /**
   * @brief This function is implemented by the user's implementation of the
   * chosen Rendering API and its purpose is to generate Scenes to render and
   * push them onto the Renderer's Scene stack.
   * @details Called by Renderer::run() after init_renderer()
   */
  virtual void startup() {}
  /**
   * @brief This function is implemented by the user's implementation of the
   * chosen Rendering API and its purpose is to cleanup anything created by the
   * Renderer at startup().
   * @details Called by Renderer::run() after the render loop ends.
   */
  virtual void shutdown() {}
  /**
   * @brief A function to end the render loop.
   * @details Can be called from anywhere after the render loop has started.
   */
  static void end_renderer();

  /**
   * @brief Get the RendererInfo object of the Renderer.
   *
   * @return The RendererInfo of the Renderer
   * @see RendererInfo
   */
  static RendererInfo &get_info();
  /**
   * @brief Get the RendererInput object of the Renderer.
   *
   * @return The RendererInput of the Renderer.
   * @see RendererInput
   */
  static RendererInput &get_input();

  /**
   * @brief Load a Scene.
   * @details On Scene load, any previously loaded scene will have it's
   * on_exit() method called after every Layer on it's Layer stack has it's
   * on_exit() method called in reverse order. Then, the new Scene's on_enter()
   * method will be called followed by the on_enter() method of every Layer on
   * the Scene's Layer stack in order of the stack.
   * @param scene A pointer to the scene to load.
   */
  static void load_scene(Scene *scene);
  /**
   * @brief Load a Scene based on it's index in the Renderer's Scene stack.
   * @details Similar to the funciton of the same name that takes a pointer to a
   * Scene.
   * @param index The index into the Scene stack to load.
   */
  static void load_scene(int index);
  /**
   * @brief Set the operating system's clipboad to a string.
   *
   * @param str The string to set the clipboard to.
   */
  virtual void api_set_clipboard_string(std::string str) = 0;
  /**
   * @brief Get a string from the operating system's clipboard if it can be
   * converted to a string.
   *
   * @return The string on the clipboard, empty if the clipbaord cannot be
   * converted to a string or if an error occured.
   */
  virtual std::string api_get_clipboard_string() = 0;
  /**
   * @brief Interface for setting the window title implemented by the Rendering
   * API.
   *
   * @param title The title to set.
   */
  virtual void api_set_window_title(const char *title) = 0;
  /**
   * @brief Interface for setting the cursor implemented by the Rendering API.
   *
   * @param type The CursorType to set.
   */
  virtual void api_set_cursor(CursorType type) = 0;
  /**
   * @brief Interface for clearing the color buffer implemented by the Rendering
   * API.
   * @details Clear the color buffer of the framebuffer to a single color.
   *
   * @param color The color to clear the color buffer to.
   */
  virtual void api_clear_color_buffer(glm::vec4 color) = 0;
  /**
   * @brief Interface for clearing the depth buffer implemented by the Rendering
   * API.
   * @details Clear the depth buffer of the framebuffer to 1.
   */
  virtual void api_clear_depth_buffer() = 0;
  /**
   * @brief Interface to resize the rendering viewport implemented by the
   * Rendering API.
   * @details Used to resize the rendering viewport when the window size changes
   * or to render into framebuffers of sizes other than the window size.
   *
   * @param width The width of the viewport in fragments or pixels.
   * @param height The height of the viewport in fragments or pixels.
   */
  virtual void api_resize_viewport(int width, int height) = 0;
  /**
   * @brief Enable or disable wireframe rendering, implemented by the Rendering
   * API.
   *
   * @param wireframe true enables wireframe rendering and false disables
   * wireframe rendering.
   */
  virtual void api_wireframe_mode(bool wireframe) = 0;
  /**
   * @brief Enable or disable depth testing, implemented by the Rendering API.
   * @details Used to enable depth testing for a Scene. Usually execued in the
   * Scene::render(float).
   * @param enable true enables depth testing and false disables depth testing.
   */
  virtual void api_enable_depth_testing(bool enable) = 0;
  /**
   * @brief Enable or disable face culling, implemented by the Rendering API.
   * @details When enabled, only triangles drawn in the  counter-clockwise
   * direction are rendered.
   * @param enable true enables face culling and false disables face culling.
   */
  virtual void api_enable_face_culling(bool enable) = 0;
  /**
   * @brief Enable or disable blending, implemented by the Rendering API.
   * @details Default blending method is one minus source alpha.
   * @param enable true enables blending and false disables blending.
   */
  virtual void api_enable_blending(bool enable) = 0;
  /**
   * @brief Raycast from the mouse position into the Camera's view of a Scene or
   * Layer. Implemented by the Rendering API.
   * @details The current depth-buffer is used to calculate the 3D point in
   * world space so it must be rendered from the same view as the Camera passed
   * to this function.
   * @param camera The Camera to raycast from.
   * @return The 3D coordinates of the Scene or Layer in world space of the
   * point along the ray intersecting the depth-buffer.
   */
  virtual glm::vec3 api_raycast(Camera *camera) = 0;
  /**
   * @brief Raycast from screen coordinates using the Camera's view into a
   * Scene or Layer. Implemented by the Rendering API.
   * @details The current depth-buffer is used to calculate the 3D point in
   * world space so it must be rendered from the same view as the Camera passed
   * to this function. The ray will appear to intersect the point on the screen
   * given by the screen coorindates
   * @param camera The Camera to raycast from.
   * @param screen_coords The screen coordinates measured in pixels from the top
   * left corner of the window.
   * @return The 3D coordinates of the Scene or Layer in world space of the
   * point along the ray that intersects the depth-buffer.
   */
  virtual glm::vec3 api_raycast(Camera *camera, glm::ivec2 screen_coords) = 0;
  /**
   * @brief Set a Framebuffer that is different from the default Framebuffer.
   * Implemented by the Rendering API.
   * @details Can be used to render into instead of the default Framebuffer.
   * @param framebuffer The Framebuffer to set. If nullptr, the default
   * Framebuffer is set.
   */
  virtual void api_set_framebuffer(Framebuffer *framebuffer) = 0;
  /**
   * @brief Dispatch a compute operation. Implemented by the Rendering API.
   * @details A ComputeProgram must be bound in order to run the ComputeProgram.
   * Normally the user would call the ComputeProgram::dispatch_compute(int, int,
   * int) method instead of the this function directly.
   *
   * @param x The number of global compute workgroups to dispatch in the x
   * dimension.
   * @param y The number of global compute workgroups to dispatch in the y
   * dimension.
   * @param z The number of global compute workgroups to dispatch in the z
   * dimension.
   * @see ComputeProgram
   */
  virtual void api_dispatch_compute(uint32_t x, uint32_t y, uint32_t z) = 0;
  /**
   * @brief API implemented function to render a SimpleMesh with a Material.
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   */
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material) = 0;
  /**
   * @brief API implemented function to render a SimpleMesh with a Material and
   * a parent Transform.
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   * @param parent_transform The parent Transform to render with.
   */
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material,
                                      Transform* parent_transform) = 0;
  /**
   * @brief API implemented function to render a SimpleMesh with a Material and
   * a parent Transform.
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   * @param parent_transform The parent Transform to render with.
   * @param instance_count The number of instances to render.
   * @param models The Transform Buffer containing the Transforms to be
   * used for instancing.
   */
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material,
                                      Transform* parent_transform,
                                      unsigned int instance_count,
                                      Buffer<Transform> *models) = 0;
  /**
   * @brief API implemented function to bind a SimpleMesh's render state to a
   * Material.
   *
   * @param mesh The SimpleMesh whose render state will be bound to the
   * Material.
   * @param material The Material the bind to.
   */
  virtual void api_bind_mesh_render_state(SimpleMesh *mesh,
                                          Material *material) = 0;
  /**
   * @brief API implemented function to destroy any render state of a
   * SimpleMesh.
   *
   * @param mesh The SimpleMesh whose render state will be destroyed.
   */
  virtual void api_destroy_mesh_render_states(SimpleMesh *mesh) = 0;
  /**
   * @brief API implemented function to push a Geometry Buffer onto a
   * SimpleMesh's Geometry Buffer stack.
   *
   * @param mesh The SimpleMesh that will have the Geometry Buffer pushed to it.
   * @param geometry_buffer The Geometry Buffer to push.
   */
  virtual void
  api_push_mesh_geometry_buffer(SimpleMesh *mesh,
                                Referenced<Buffer<float>> geometry_buffer) = 0;
  /**
   * @brief API implemented function to set the Index Buffer of a SimpleMesh.
   *
   * @param mesh The SimpleMesh that will have its Index Buffer set.
   * @param index_buffer The Index Buffer to set.
   */
  virtual void
  api_set_mesh_index_buffer(SimpleMesh *mesh,
                            Referenced<Buffer<uint32_t>> index_buffer) = 0;
  /**
   * @brief API implemented function to generate a Texture2D from an image
   * filepath.
   *
   * @param image_filepath The filepath of the image. Can be relative to where
   * the application was executed of absolute.
   * @return A Scoped Texture2D.
   */
  virtual Scoped<Texture2D> api_gen_texture2D(const char *image_filepath) = 0;
  /**
   * @brief API implemented function to generate a blank Texture2D for writing
   * to.
   *
   * @param type The TextureType to use.
   * @param width The width in pixels or fragments of the Texture2D.
   * @param height the height in pixels or fragments of the Texture2D.
   * @return A Scoped Texture2D.
   * @see TextureType
   */
  virtual Scoped<Texture2D> api_gen_texture2D(TextureType type, int width,
                                              int height) = 0;
  /**
   * @brief API implemented function to generate a blank Framebuffer to render
   * into.
   *
   * @param width The width in pixels or fragments of the Framebuffer.
   * @param height The height in pixels or fragments of the Framebuffer.
   * @return Scoped<Framebuffer>
   */
  virtual Scoped<Framebuffer> api_gen_framebuffer(int width, int height) = 0;
  /**
   * @brief API implemented function to generate a Shader from a directory.
   * @details The directory must contain the glsl files that the shader program
   * consists of with the appropriate extensions.
   *
   * @param directory The directory where the glsl files are stored.
   * @return A Scoped Shader.
   * @see Shader
   */
  virtual Scoped<Shader> api_gen_shader(const char *directory) = 0;

  /**
   * @brief Static access to Renderer::api_set_window_title(const char*).
   *
   * @param title The title to set.
   * @see Renderer::api_set_window_title(const char*)
   */
  static void set_window_title(const char *title) {
    API->api_set_window_title(title);
  }
  /**
   * @brief Static access to Renderer::api_set_clipboard_string(std::string).
   *
   * @param str The string to set the clipboard to.
   */
  static void set_clipboard_string(std::string str) {
    return API->api_set_clipboard_string(str);
  }
  /**
   * @brief Static access to Renderer::api_get_clipboard_string().
   *
   * @return The string on the clipboard, empty if the clipbaord cannot be
   * converted to a string or if an error occured.
   */
  static std::string get_clipboard_string() {
    return API->api_get_clipboard_string();
  }
  /**
   * @brief Static access to Renderer::api_set_cursor(CursorType).
   *
   * @param type The CursorType
   * @see Renderer::api_set_cursor(CursorType)
   * @see CursorType
   */
  static void set_cursor(CursorType type) { API->api_set_cursor(type); }
  /**
   * @brief Static access to Renderer::api_clear_color_buffer(glm::vec4).
   *
   * @param color The color to clear the color buffer to.
   * @see Renderer::api_clear_color_buffer(glm::vec4)
   */
  static void clear_color_buffer(glm::vec4 color) {
    API->api_clear_color_buffer(color);
  }
  /**
   * @brief Static access to Renderer::api_clear_depth_buffer().
   * @see Renderer::api_clear_depth_buffer()
   */
  static void clear_depth_buffer() { API->api_clear_depth_buffer(); }
  /**
   * @brief Static access to Renderer::api_resize_viewport(int,int).
   *
   * @param width Width in pixels to resize the viewport to.
   * @param height Height in pixels to resize the viewport to.
   * @see Renderer::api_resize_viewport(int,int)
   */
  static void resize_viewport(int width, int height) {
    API->api_resize_viewport(width, height);
  }
  /**
   * @brief Static access to Renderer::api_wireframe_mode(bool).
   *
   * @param wireframe true enables wireframe rendering, false disables wireframe
   * rendering.
   * @see Renderer::api_wireframe_mode(bool)
   */
  static void wireframe_mode(bool wireframe) {
    API->api_wireframe_mode(wireframe);
  }
  /**
   * @brief Static access to Renderer::api_enable_depth_testing(bool).
   *
   * @param enable true enables depth testing, false disables depth testing.
   * @see Renderer::api_enable_depth_testing(bool)
   */
  static void enable_depth_testing(bool enable) {
    API->api_enable_depth_testing(enable);
  }
  /**
   * @brief Static access to Renderer::api_enable_face_culling(bool).
   *
   * @param enable true enables face culling, false disables face culling.
   * @see Renderer::api_enable_face_culling(bool)
   */
  static void enable_face_culling(bool enable) {
    API->api_enable_face_culling(enable);
  }
  /**
   * @brief Static access to Renderer::api_enable_blending(bool).
   *
   * @param enable true enables blending, false disables blending.
   * @see Renderer::api_enable_blending(bool)
   */
  static void enable_blending(bool enable) { API->api_enable_blending(enable); }
  /**
   * @brief Static access to Renderer::api_raycast(Camera*).
   *
   * @param camera The Camera to raycast from.
   * @return The 3D coordinates in world space of the ray intersecting with the
   * depth buffer.
   * @see Renderer::api_raycast(Camera*)
   */
  static glm::vec3 raycast(Camera *camera) { return API->api_raycast(camera); }
  /**
   * @brief Static access to Renderer::api_raycast(Camera*, glm::ivec2).
   *
   * @param camera The Camera to raycast from.
   * @param screen_coords The sceen coordinates of the ray.
   * @return The 3D coordinates in world space of the ray intersecting with the
   * depth buffer.
   * @see Renderer::api_raycast(Camera*, glm::ivec2)
   */
  static glm::vec3 raycast(Camera *camera, glm::ivec2 screen_coords) {
    return API->api_raycast(camera, screen_coords);
  }
  /**
   * @brief Static access to Renderer::api_set_framebuffer(Framebuffer*).
   *
   * @param framebuffer The Framebuffer to set.
   * @see Renderer::api_set_framebuffer(Framebuffer*)
   */
  static void set_framebuffer(Framebuffer *framebuffer) {
    API->api_set_framebuffer(framebuffer);
  }
  /**
   * @brief Static access to Renderer::api_dispatch_compute(uint32_t, uint32_t,
   * uint32_t).
   *
   * @param x The number of x workgroups to dispatch.
   * @param y The number of y workgroups to dispatch.
   * @param z The number of z workgroups to dispatch.
   * @see Renderer::api_dispatch_compute(uint32_t, uint32_t, uint32_t)
   */
  static void dispatch_compute(uint32_t x, uint32_t y, uint32_t z) {
    API->api_dispatch_compute(x, y, z);
  }
  /**
   * @brief Static access to Renderer::api_renderer_simple_mesh(Camera*,
   * SimpleMesh*, Material*).
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   * @see Renderer::api_renderer_simple_mesh(Camera*, SimpleMesh*, Material*)
   */
  static void render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                 Material *material) {
    API->api_render_simple_mesh(camera, mesh, material);
  }
  /**
   * @brief Static access to Renderer::api_renderer_simple_mesh(Camera*,
   * SimpleMesh*, Material*, glm::mat4&).
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   * @param parent_transform The parent Transform to render with.
   * @see Renderer::api_renderer_simple_mesh(Camera*, SimpleMesh*, Material*,
   * glm::mat4&)
   */
  static void render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                 Material *material, Transform* parent_transform) {
    API->api_render_simple_mesh(camera, mesh, material, parent_transform);
  }
  /**
   * @brief Static access to Renderer::api_renderer_simple_mesh(Camera*,
   * SimpleMesh*, Material*, glm::mat4&, unsigned int, Buffer<glm::mat4>*).
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   * @param parent_transform The parent Transform to render with.
   * @param instance_count The number of instances to render.
   * @param models The Transform Buffer containing the Transforms to be
   * used for instancing.
   * @see Renderer::api_renderer_simple_mesh(Camera*, SimpleMesh*, Material*,
   * glm::mat4&, unsigned int, Buffer<glm::mat4>*)
   */
  static void render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                 Material *material, Transform* parent_transform,
                                 unsigned int instance_count,
                                 Buffer<Transform> *models) {
    API->api_render_simple_mesh(camera, mesh, material, parent_transform,
                                instance_count, models);
  }
  /**
   * @brief Static access to Renderer::api_bind_mesh_render_state(SimpleMesh*,
   * Material*).
   *
   * @param mesh The SimpleMesh whose render state will be bound to the
   * Material.
   * @param material The Material to bind to.
   * @see Renderer::api_bind_mesh_render_state(SimpleMesh*, Material*)
   */
  static void bind_mesh_render_state(SimpleMesh *mesh, Material *material) {
    API->api_bind_mesh_render_state(mesh, material);
  }
  /**
   * @brief Static access to
   * Renderer::api_destroy_mesh_render_states(SimpleMesh*).
   *
   * @param mesh The SimpleMesh whose render state will be destroyed.
   * @see Renderer::api_destroy_mesh_render_states(SimpleMesh*)
   */
  static void destroy_mesh_render_states(SimpleMesh *mesh) {
    API->api_destroy_mesh_render_states(mesh);
  }
  /**
   * @brief Static access to
   * Renderer::push_mesh_geometry_buffer(SimpleMesh*,Referenced<Buffer<float>>).
   *
   * @param mesh The SimpleMesh to push the Geometry Buffer to.
   * @param geometry_buffer The Geometry Buffer to push.
   * @see
   * Renderer::push_mesh_geometry_buffer(SimpleMesh*,Referenced<Buffer<float>>)
   */
  static void
  push_mesh_geometry_buffer(SimpleMesh *mesh,
                            Referenced<Buffer<float>> geometry_buffer) {
    API->api_push_mesh_geometry_buffer(mesh, geometry_buffer);
  }
  /**
   * @brief Static access to Renderer::set_mesh_index_buffer(SimpleMesh*,
   * Referenced<Buffer<uint32_t>>)
   *
   * @param mesh The SimpleMesh whose Index Buffer will be set.
   * @param index_buffer The Index Buffer to set.
   * @see Renderer::set_mesh_index_buffer(SimpleMesh*,
   * Referenced<Buffer<uint32_t>>)
   */
  static void set_mesh_index_buffer(SimpleMesh *mesh,
                                    Referenced<Buffer<uint32_t>> index_buffer) {
    API->api_set_mesh_index_buffer(mesh, index_buffer);
  }
  /**
   * @brief Static access to Renderer::gen_texture2D(const char*).
   *
   * @param image_filepath Filepath to the image file.
   * @return Scoped Texture2D
   * @see Renderer::gen_texture2D(const char*)
   */
  static Scoped<Texture2D> gen_texture2D(const char *image_filepath) {
    return API->api_gen_texture2D(image_filepath);
  }
  /**
   * @brief Static access to Renderer::gen_texture2D(TextureType, int, int).
   *
   * @param type The TextureType
   * @param width The width of the Texture2D.
   * @param height The height of the Texture2D.
   * @return Scoped Texture2D
   * @see Renderer::gen_texture2D(TextureType, int, int)
   */
  static Scoped<Texture2D> gen_texture2D(TextureType type, int width,
                                         int height) {
    return API->api_gen_texture2D(type, width, height);
  }
  /**
   * @brief Static access to Renderer::gen_framebuffer(int, int)
   *
   * @param width Width of the Framebuffer.
   * @param height Height of the Framebuffer.
   * @return Scoped Framebuffer
   * @see Renderer::gen_framebuffer(int, int)
   */
  static Scoped<Framebuffer> gen_framebuffer(int width, int height) {
    return API->api_gen_framebuffer(width, height);
  }
  /**
   * @brief Static access to Renderer::gen_shader(const char*).
   *
   * @param directory The directory of the shader program files.
   * @return Scoped Shader
   * @see Renderer::gen_shader(const char*)
   */
  static Scoped<Shader> gen_shader(const char *directory) {
    return API->api_gen_shader(directory);
  }

  /**
   * @brief Function to generate a Buffer of type <T>.
   *
   * @tparam <T> The type of data contained in the Buffer.
   * @param data A pointer to the data used to initialized the Buffer. Can be
   * nullptr if BufferType is not BufferType::STATIC.
   * @param size_in_bytes The size in bytes allocated for the Buffer.
   * @param buffer_type The BufferType.
   * @return Scoped Buffer<T>
   * @see BufferType
   * @see Buffer<T>
   */
  template <typename T>
  static Scoped<Buffer<T>>
  gen_buffer(T *data, size_t size_in_bytes,
             BufferType buffer_type = BufferType::STATIC) {
    switch (info.API) {
    case RendererAPI::OpenGL_4_5:
      return std::make_unique<GLBuffer<T>>(data, size_in_bytes, buffer_type);
      break;
    default:
      return nullptr;
    }
  }

  /**
   * @brief Generate a Scene and push it onto the Scene stack.
   * @details This function will generate a new reference to a Scene of type
   * and push it onto the Renderer's Scene stack.
   *
   * @tparam <T> The type of Scene. <T> must be a Scene.
   * @tparam <Args> The type of the arguments for the Contructor of the Scene.
   * @param args The arguments for the Constructor of the Scene.
   * @return The Referenced Scene.
   * @see Scene
   */
  template <typename T, typename... Args>
  static Referenced<T> gen_scene(Args... args) {
    // generate scene reference of type T and cast to base type
    Referenced<T> scn = gen_ref<T>(args...);
    scenes_.push_back(scn);
    return scn;
  }
  /**
   * @brief Generate multiple Scenes and push them onto the Scene stack.
   * @details This function will create multiple Scenes of the same type and
   * push them all onto the Scene stack. A `std::vector` of Referenced Scenes
   * is returned that can be used and will be deleted when it goes out of
   * scope.
   *
   * @tparam <T> The type of Scenes to generate. <T> must be a Scene.
   * @tparam <Args> The type of the arguments for the Contructor of the Scene.
   * @param args The arguments for the Constructor of the Scene.
   * @param count The number of Scenes to generate.
   * @see Scene
   */
  template <typename T, typename... Args>
  static std::vector<Referenced<T>> gen_scenes(uint32_t count, Args... args) {
    std::vector<Referenced<T>> scenes{};
    for (uint32_t i = 0; i < count; i++) {
      // generate scene reference of type T and cast to base type
      Referenced<T> scn = gen_ref<T>(args...);
      // if T is a scene push the scene onto the stack
      if (scn) {
        scenes_.push_back(scn);
        scenes.push_back(scn);
      }
    }
    return scenes;
  }
  /**
   * @brief Get a pointer to the first Scene of type <T> in the Renderer's
   * Scene stack.
   *
   * @tparam <T> The type of Scene to get. <T> must be a Scene.
   * @return A pointer to the first Scene of type <T> in the Scene stack.
   * nullptr is returned if no Scene of type <T> exists on the Scene stack.
   * @see Scene
   */
  template <typename T> static T *get_scene() {
    for (auto scn_it = scene_begin(); scn_it != scene_end(); scn_it++) {
      if (auto scene = std::dynamic_pointer_cast<T>(*scn_it)) {
        return scene.get();
      }
    }
    return nullptr;
  }
  /**
   * @brief Get a `std::vector` of pointers to all of the Scenes of type <T> in
   * the Renderer's Scene stack.
   *
   * @tparam <T> The type of Scene to get. <T> must be a Scene.
   * @return A `std::vector` of pointers to all of the Scenes of type <T> in
   * the Scene stack. An empty vector is returned if no Scene of type <T>
   * exists on the Scene stack.
   * @see Scene
   */
  template <typename T> static std::vector<T *> get_scenes() {
    std::vector<T *> scenes{};
    for (auto scn_it = scene_begin(); scn_it != scene_end(); scn_it++) {
      if (auto scene = std::dynamic_pointer_cast<T>(*scn_it)) {
        scenes.push_back(scene.get());
      }
    }
    return scenes;
  }
  /**
   * @brief Push an existing Referenced Scene onto the Renderer's Scene stack.
   *
   * @param system A Referenced Scene.
   * @see Scene
   */
  template <typename T> static void push_scene(Referenced<T> scene) {
    Referenced<Scene> scn = std::dynamic_pointer_cast<Scene>(scene);
    if (scn)
      scenes_.push_back(scn);
  }
  /**
   * @brief Push a `std::vector` of existing Referenced Scenes onto the
   * Renderer's Scene stack.
   *
   * @param systems A `std::vector` of existing Referenced Scenes.
   * @see Scene
   */
  template <typename T>
  static void push_scenes(std::vector<Referenced<T>> scenes) {
    for (size_t i = 0; i < scenes.size(); i++) {
      Referenced<Scene> scn = std::dynamic_pointer_cast<Scene>(scenes[i]);
      if (scn)
        scenes_.push_back(scn);
    }
  }
  /**
   * @brief Remove the first Scene of type <T> from the Scene stack and return
   * the Referenced Scene.
   *
   * @tparam <T> The type of Scene to pull.
   * @return The Referenced Scene that was pulled from the Scene stack.
   * @see Scene
   */
  template <typename T> static Referenced<T> pull_scene() {
    for (auto scn_it = scene_begin(); scn_it != scene_end(); scn_it++) {
      if (auto scene = std::dynamic_pointer_cast<T>(*scn_it)) {
        Referenced<T> pulled_scene = std::move(scene);
        scenes_.erase(scn_it);
        return pulled_scene;
      }
    }
    return nullptr;
  }
  /**
   * @brief Remove all of the Scenes of type <T> from the Scene stack and
   * return a `std::vector` of the Referenced Scenes.
   *
   * @tparam <T> The type of Scenes to pull.
   * @return A `std::vector` of the Referenced Scenes that were pulled from the
   * Scene stack.
   * @see Scene
   */
  template <typename T> static std::vector<Referenced<T>> pull_scenes() {
    std::vector<T *> scenes{};
    for (auto scn_it = scene_begin(); scn_it != scene_end(); scn_it++) {
      if (auto scene = std::dynamic_pointer_cast<T>(*scn_it)) {
        Referenced<T> pulled_scene = std::move(scene);
        scenes_.erase(scn_it);
        scenes.push_back(pulled_scene);
      }
    }
    return scenes;
  }

  /**
   * @brief Get a iterator pointing to the begining of the Scene stack.
   *
   * @return An iterator pointing to the begining of the Scene stack.
   * @see Scene
   */
  static std::vector<Referenced<Scene>>::iterator scene_begin() {
    return scenes_.begin();
  }
  /**
   * @brief Get a iterator pointing to the end of the Scene stack.
   *
   * @return An iterator pointing to the end of the Scene stack.
   * @see Scene
   */
  static std::vector<Referenced<Scene>>::iterator scene_end() {
    return scenes_.end();
  }
  /**
   * @brief Get a reverse iterator pointing to the end of the Scene
   * stack.
   *
   * @return A reverse iterator pointing to the end of the Scene stack.
   * @see Scene
   */
  static std::vector<Referenced<Scene>>::reverse_iterator scene_rbegin() {
    return scenes_.rbegin();
  }
  /**
   * @brief Get a reverse iterator pointing to the begining of the Scene
   * stack.
   *
   * @return A reverse iterator pointing to the begining of the Scene stack.
   * @see Scene
   */
  static std::vector<Referenced<Scene>>::reverse_iterator scene_rend() {
    return scenes_.rend();
  }

  /**
   * @brief Set the Renderer API.
   * @details This sets the static Renderer::API variable. The Renderer API
   * implementation will use this.
   *
   * @param renderer The Renderer API to set.
   */
  static void set_renderer(Renderer *renderer) { API = renderer; }

protected:
  // static variables for renderer
  static RendererInfo
      info; /**< The RendererInfo for the implemented Renderer.*/
  static RendererInput input; /**< The RendererInput.*/
  static bool running;  /**< true after Renderer initialization and when the
                           render loop is running. false when
                           the render loop has been signaled to end.*/
  static Renderer *API; /**< The implemented Rendering API.*/
  static std::vector<Referenced<Scene>> scenes_; /**< The Scene stack.*/
};

/**
 * @brief A function used to launch an application.
 * @details The application must be a class inherited from an implementation of
 * a Rendering API.
 *
 * @tparam <T> The type of application to launch. Must be a type of Renderer.
 * @return Returns 0 if successfully closed and 1 is an error occured.
 * @see Renderer
 * @see GLRenderer
 */
template <typename T> int launch() {
  static_assert(std::is_base_of<Renderer, T>::value,
                "Type must be a renderer.");
  try {
    std::unique_ptr<T> renderer = std::make_unique<T>();
    Renderer::set_renderer(renderer.get());
    renderer->run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}

} // namespace mare

#endif