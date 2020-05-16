#ifndef RENDERER
#define RENDERER

// Standard Library
#include <bitset>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>

// MARE
#include "mare/Buffers.hpp"
#include "mare/GL/GLBuffers.hpp"
#include "mare/Mare.hpp"
#include "mare/Shader.hpp"

namespace mare {
// Forward Declarations
class SimpleMesh;
class Layer;
class Camera;
class Scene;

// The available cursors for the application to use
enum class CURSOR { DISABLED, ENABLED, ARROW, HZ_RESIZE, HAND, CROSSHAIRS };

enum class RendererAPI {
  NONE = 0,
  OpenGL_4_5,
  DirectX_12, // Not yet supported
  Vulkan      // Not yet supported
};

// This info is set after Renderer creation but before the renderer executes
struct RendererInfo {
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
  std::bitset<4> debug_mode{}; // 0000 == off, 0001 == high, 0010 == med, 0100
                               // == low, 1000 == notification
  RendererAPI API{RendererAPI::NONE};
};

// This input is passed by reference to the callbacks which decide what to do
// with it
struct RendererInput {
  std::bitset<3> mouse_button{}; // 001 == left, 010 == right, 100 == middle
  short mouse_scroll{};   // 0 == no scroll, 1 == scroll up, -1 = scroll down
  glm::ivec2 mouse_pos{}; // window coordinates of mouse position (in pixels
                          // from top left corner)
  glm::ivec2 mouse_vel{}; // window coordinates of mouse velocity (in pixels
                          // from top left corner)
  // Key presses
  bool SPACE_PRESSED{false};
  bool SPACE_JUST_PRESSED{false};
  bool SPACE_RELEASED{true};
  bool APOSTROPHE_PRESSED{false};
  bool APOSTROPHE_JUST_PRESSED{false};
  bool APOSTROPHE_RELEASED{true};
  bool COMMA_PRESSED{false};
  bool COMMA_JUST_PRESSED{false};
  bool COMMA_RELEASED{true};
  bool MINUS_PRESSED{false};
  bool MINUS_JUST_PRESSED{false};
  bool MINUS_RELEASED{true};
  bool PERIOD_PRESSED{false};
  bool PERIOD_JUST_PRESSED{false};
  bool PERIOD_RELEASED{true};
  bool FORWARD_SLASH_PRESSED{false};
  bool FORWARD_SLASH_JUST_PRESSED{false};
  bool FORWARD_SLASH_RELEASED{true};
  bool ZERO_PRESSED{false};
  bool ZERO_JUST_PRESSED{false};
  bool ZERO_RELEASED{true};
  bool ONE_PRESSED{false};
  bool ONE_JUST_PRESSED{false};
  bool ONE_RELEASED{true};
  bool TWO_PRESSED{false};
  bool TWO_JUST_PRESSED{false};
  bool TWO_RELEASED{true};
  bool THREE_PRESSED{false};
  bool THREE_JUST_PRESSED{false};
  bool THREE_RELEASED{true};
  bool FOUR_PRESSED{false};
  bool FOUR_JUST_PRESSED{false};
  bool FOUR_RELEASED{true};
  bool FIVE_PRESSED{false};
  bool FIVE_JUST_PRESSED{false};
  bool FIVE_RELEASED{true};
  bool SIX_PRESSED{false};
  bool SIX_JUST_PRESSED{false};
  bool SIX_RELEASED{true};
  bool SEVEN_PRESSED{false};
  bool SEVEN_JUST_PRESSED{false};
  bool SEVEN_RELEASED{true};
  bool EIGHT_PRESSED{false};
  bool EIGHT_JUST_PRESSED{false};
  bool EIGHT_RELEASED{true};
  bool NINE_PRESSED{false};
  bool NINE_JUST_PRESSED{false};
  bool NINE_RELEASED{true};
  bool SEMICOLON_PRESSED{false};
  bool SEMICOLON_JUST_PRESSED{false};
  bool SEMICOLON_RELEASED{true};
  bool EQUAL_PRESSED{false};
  bool EQUAL_JUST_PRESSED{false};
  bool EQUAL_RELEASED{true};
  bool A_PRESSED{false};
  bool A_JUST_PRESSED{false};
  bool A_RELEASED{true};
  bool B_PRESSED{false};
  bool B_JUST_PRESSED{false};
  bool B_RELEASED{true};
  bool C_PRESSED{false};
  bool C_JUST_PRESSED{false};
  bool C_RELEASED{true};
  bool D_PRESSED{false};
  bool D_JUST_PRESSED{false};
  bool D_RELEASED{true};
  bool E_PRESSED{false};
  bool E_JUST_PRESSED{false};
  bool E_RELEASED{true};
  bool F_PRESSED{false};
  bool F_JUST_PRESSED{false};
  bool F_RELEASED{true};
  bool G_PRESSED{false};
  bool G_JUST_PRESSED{false};
  bool G_RELEASED{true};
  bool H_PRESSED{false};
  bool H_JUST_PRESSED{false};
  bool H_RELEASED{true};
  bool I_PRESSED{false};
  bool I_JUST_PRESSED{false};
  bool I_RELEASED{true};
  bool J_PRESSED{false};
  bool J_JUST_PRESSED{false};
  bool J_RELEASED{true};
  bool K_PRESSED{false};
  bool K_JUST_PRESSED{false};
  bool K_RELEASED{true};
  bool L_PRESSED{false};
  bool L_JUST_PRESSED{false};
  bool L_RELEASED{true};
  bool M_PRESSED{false};
  bool M_JUST_PRESSED{false};
  bool M_RELEASED{true};
  bool N_PRESSED{false};
  bool N_JUST_PRESSED{false};
  bool N_RELEASED{true};
  bool O_PRESSED{false};
  bool O_JUST_PRESSED{false};
  bool O_RELEASED{true};
  bool P_PRESSED{false};
  bool P_JUST_PRESSED{false};
  bool P_RELEASED{true};
  bool Q_PRESSED{false};
  bool Q_JUST_PRESSED{false};
  bool Q_RELEASED{true};
  bool R_PRESSED{false};
  bool R_JUST_PRESSED{false};
  bool R_RELEASED{true};
  bool S_PRESSED{false};
  bool S_JUST_PRESSED{false};
  bool S_RELEASED{true};
  bool T_PRESSED{false};
  bool T_JUST_PRESSED{false};
  bool T_RELEASED{true};
  bool U_PRESSED{false};
  bool U_JUST_PRESSED{false};
  bool U_RELEASED{true};
  bool V_PRESSED{false};
  bool V_JUST_PRESSED{false};
  bool V_RELEASED{true};
  bool W_PRESSED{false};
  bool W_JUST_PRESSED{false};
  bool W_RELEASED{true};
  bool X_PRESSED{false};
  bool X_JUST_PRESSED{false};
  bool X_RELEASED{true};
  bool Y_PRESSED{false};
  bool Y_JUST_PRESSED{false};
  bool Y_RELEASED{true};
  bool Z_PRESSED{false};
  bool Z_JUST_PRESSED{false};
  bool Z_RELEASED{true};
  bool LEFT_BRACKET_PRESSED{false};
  bool LEFT_BRACKET_JUST_PRESSED{false};
  bool LEFT_BRACKET_RELEASED{true};
  bool RIGHT_BRACKET_PRESSED{false};
  bool RIGHT_BRACKET_JUST_PRESSED{false};
  bool RIGHT_BRACKET_RELEASED{true};
  bool GRAVE_ACCENT_PRESSED{false};
  bool GRAVE_ACCENT_JUST_PRESSED{false};
  bool GRAVE_ACCENT_RELEASED{true};
  bool ESCAPE_PRESSED{false};
  bool ESCAPE_JUST_PRESSED{false};
  bool ESCAPE_RELEASED{true};
  bool ENTER_PRESSED{false};
  bool ENTER_JUST_PRESSED{false};
  bool ENTER_RELEASED{true};
  bool TAB_PRESSED{false};
  bool TAB_JUST_PRESSED{false};
  bool TAB_RELEASED{true};
  bool BACKSPACE_PRESSED{false};
  bool BACKSPACE_JUST_PRESSED{false};
  bool BACKSPACE_RELEASED{true};
  bool INSERT_PRESSED{false};
  bool INSERT_JUST_PRESSED{false};
  bool INSERT_RELEASED{true};
  bool DELETE_PRESSED{false};
  bool DELETE_JUST_PRESSED{false};
  bool DELETE_RELEASED{true};
  bool RIGHT_PRESSED{false};
  bool RIGHT_JUST_PRESSED{false};
  bool RIGHT_RELEASED{true};
  bool LEFT_PRESSED{false};
  bool LEFT_JUST_PRESSED{false};
  bool LEFT_RELEASED{true};
  bool UP_PRESSED{false};
  bool UP_JUST_PRESSED{false};
  bool UP_RELEASED{true};
  bool DOWN_PRESSED{false};
  bool DOWN_JUST_PRESSED{false};
  bool DOWN_RELEASED{true};
  bool PAGE_UP_PRESSED{false};
  bool PAGE_UP_JUST_PRESSED{false};
  bool PAGE_UP_RELEASED{true};
  bool PAGE_DOWN_PRESSED{false};
  bool PAGE_DOWN_JUST_PRESSED{false};
  bool PAGE_DOWN_RELEASED{true};
  bool HOME_PRESSED{false};
  bool HOME_JUST_PRESSED{false};
  bool HOME_RELEASED{true};
  bool END_PRESSED{false};
  bool END_JUST_PRESSED{false};
  bool END_RELEASED{true};
  bool CAPS_LOCK_PRESSED{false};
  bool CAPS_LOCK_JUST_PRESSED{false};
  bool CAPS_LOCK_RELEASED{true};
  bool SCROLL_LOCK_PRESSED{false};
  bool SCROLL_LOCK_JUST_PRESSED{false};
  bool SCROLL_LOCK_RELEASED{true};
  bool NUM_LOCK_PRESSED{false};
  bool NUM_LOCK_JUST_PRESSED{false};
  bool NUM_LOCK_RELEASED{true};
  bool PRINT_SCREEN_PRESSED{false};
  bool PRINT_SCREEN_JUST_PRESSED{false};
  bool PRINT_SCREEN_RELEASED{true};
  bool PAUSE_PRESSED{false};
  bool PAUSE_JUST_PRESSED{false};
  bool PAUSE_RELEASED{true};
  bool F1_PRESSED{false};
  bool F1_JUST_PRESSED{false};
  bool F1_RELEASED{true};
  bool F2_PRESSED{false};
  bool F2_JUST_PRESSED{false};
  bool F2_RELEASED{true};
  bool F3_PRESSED{false};
  bool F3_JUST_PRESSED{false};
  bool F3_RELEASED{true};
  bool F4_PRESSED{false};
  bool F4_JUST_PRESSED{false};
  bool F4_RELEASED{true};
  bool F5_PRESSED{false};
  bool F5_JUST_PRESSED{false};
  bool F5_RELEASED{true};
  bool F6_PRESSED{false};
  bool F6_JUST_PRESSED{false};
  bool F6_RELEASED{true};
  bool F7_PRESSED{false};
  bool F7_JUST_PRESSED{false};
  bool F7_RELEASED{true};
  bool F8_PRESSED{false};
  bool F8_JUST_PRESSED{false};
  bool F8_RELEASED{true};
  bool F9_PRESSED{false};
  bool F9_JUST_PRESSED{false};
  bool F9_RELEASED{true};
  bool F10_PRESSED{false};
  bool F10_JUST_PRESSED{false};
  bool F10_RELEASED{true};
  bool F11_PRESSED{false};
  bool F11_JUST_PRESSED{false};
  bool F11_RELEASED{true};
  bool F12_PRESSED{false};
  bool F12_JUST_PRESSED{false};
  bool F12_RELEASED{true};
  bool KEY_PAD_0_PRESSED{false};
  bool KEY_PAD_0_JUST_PRESSED{false};
  bool KEY_PAD_0_RELEASED{true};
  bool KEY_PAD_1_PRESSED{false};
  bool KEY_PAD_1_JUST_PRESSED{false};
  bool KEY_PAD_1_RELEASED{true};
  bool KEY_PAD_2_PRESSED{false};
  bool KEY_PAD_2_JUST_PRESSED{false};
  bool KEY_PAD_2_RELEASED{true};
  bool KEY_PAD_3_PRESSED{false};
  bool KEY_PAD_3_JUST_PRESSED{false};
  bool KEY_PAD_3_RELEASED{true};
  bool KEY_PAD_4_PRESSED{false};
  bool KEY_PAD_4_JUST_PRESSED{false};
  bool KEY_PAD_4_RELEASED{true};
  bool KEY_PAD_5_PRESSED{false};
  bool KEY_PAD_5_JUST_PRESSED{false};
  bool KEY_PAD_5_RELEASED{true};
  bool KEY_PAD_6_PRESSED{false};
  bool KEY_PAD_6_JUST_PRESSED{false};
  bool KEY_PAD_6_RELEASED{true};
  bool KEY_PAD_7_PRESSED{false};
  bool KEY_PAD_7_JUST_PRESSED{false};
  bool KEY_PAD_7_RELEASED{true};
  bool KEY_PAD_8_PRESSED{false};
  bool KEY_PAD_8_JUST_PRESSED{false};
  bool KEY_PAD_8_RELEASED{true};
  bool KEY_PAD_9_PRESSED{false};
  bool KEY_PAD_9_JUST_PRESSED{false};
  bool KEY_PAD_9_RELEASED{true};
  bool KEY_PAD_DECIMAL_PRESSED{false};
  bool KEY_PAD_DECIMAL_JUST_PRESSED{false};
  bool KEY_PAD_DECIMAL_RELEASED{true};
  bool KEY_PAD_DIVIDE_PRESSED{false};
  bool KEY_PAD_DIVIDE_JUST_PRESSED{false};
  bool KEY_PAD_DIVIDE_RELEASED{true};
  bool KEY_PAD_MULTIPLY_PRESSED{false};
  bool KEY_PAD_MULTIPLY_JUST_PRESSED{false};
  bool KEY_PAD_MULTIPLY_RELEASED{true};
  bool KEY_PAD_SUBTRACT_PRESSED{false};
  bool KEY_PAD_SUBTRACT_JUST_PRESSED{false};
  bool KEY_PAD_SUBTRACT_RELEASED{true};
  bool KEY_PAD_ADD_PRESSED{false};
  bool KEY_PAD_ADD_JUST_PRESSED{false};
  bool KEY_PAD_ADD_RELEASED{true};
  bool KEY_PAD_ENTER_PRESSED{false};
  bool KEY_PAD_ENTER_JUST_PRESSED{false};
  bool KEY_PAD_ENTER_RELEASED{true};
  bool KEY_PAD_EQUAL_PRESSED{false};
  bool KEY_PAD_EQUAL_JUST_PRESSED{false};
  bool KEY_PAD_EQUAL_RELEASED{true};
  bool LEFT_SHIFT_PRESSED{false};
  bool LEFT_SHIFT_JUST_PRESSED{false};
  bool LEFT_SHIFT_RELEASED{true};
  bool LEFT_CONTROL_PRESSED{false};
  bool LEFT_CONTROL_JUST_PRESSED{false};
  bool LEFT_CONTROL_RELEASED{true};
  bool LEFT_ALT_PRESSED{false};
  bool LEFT_ALT_JUST_PRESSED{false};
  bool LEFT_ALT_RELEASED{true};
  bool LEFT_SUPER_PRESSED{false};
  bool LEFT_SUPER_JUST_PRESSED{false};
  bool LEFT_SUPER_RELEASED{true};
  bool RIGHT_SHIFT_PRESSED{false};
  bool RIGHT_SHIFT_JUST_PRESSED{false};
  bool RIGHT_SHIFT_RELEASED{true};
  bool RIGHT_CONTROL_PRESSED{false};
  bool RIGHT_CONTROL_JUST_PRESSED{false};
  bool RIGHT_CONTROL_RELEASED{true};
  bool RIGHT_ALT_PRESSED{false};
  bool RIGHT_ALT_JUST_PRESSED{false};
  bool RIGHT_ALT_RELEASED{true};
  bool RIGHT_SUPER_PRESSED{false};
  bool RIGHT_SUPER_JUST_PRESSED{false};
  bool RIGHT_SUPER_RELEASED{true};
  bool MENU_PRESSED{false};
  bool MENU_JUST_PRESSED{false};
  bool MENU_RELEASED{true};
};

class Renderer {
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

  // Scenes
  static void load_scene(Scene *scene);
  static void load_scene(int index);

  // API implementations -----------------------------------
  // Renderer Commands
  virtual void api_set_window_title(const char *title) = 0;
  virtual void api_set_cursor(CURSOR type) = 0;
  virtual void api_clear_color_buffer(glm::vec4 color) = 0;
  virtual void api_clear_depth_buffer() = 0;
  virtual void api_resize_window(int width, int height) = 0;
  virtual void api_wireframe_mode(bool wireframe) = 0;
  virtual void api_enable_depth_testing(bool enable) = 0;
  virtual void api_enable_face_culling(bool enable) = 0;
  virtual void api_enable_blending(bool enable) = 0;
  virtual glm::vec3 api_raycast(Camera *camera) = 0;
  virtual glm::vec3 api_raycast(Camera *camera, glm::ivec2 screen_coords) = 0;
  virtual void api_set_framebuffer(Framebuffer *framebuffer) = 0;
  virtual void api_dispatch_compute(uint32_t x, uint32_t y, uint32_t z) = 0;
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material) = 0;
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material,
                                      glm::mat4 parent_model) = 0;
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material,
                                      glm::mat4 parent_model,
                                      unsigned int instance_count,
                                      Buffer<glm::mat4> *models) = 0;
  virtual void api_bind_mesh_render_state(SimpleMesh *mesh,
                                          Material *material) = 0;
  virtual void api_destroy_mesh_render_states(SimpleMesh *mesh) = 0;
  virtual void
  api_push_mesh_geometry_buffer(SimpleMesh *mesh,
                                Scoped<Buffer<float>> geometry_buffer) = 0;
  virtual void
  api_set_mesh_index_buffer(SimpleMesh *mesh,
                            Scoped<Buffer<uint32_t>> index_buffer) = 0;
  virtual Scoped<Texture2D> api_gen_texture2D(const char *image_filepath) = 0;
  virtual Scoped<Texture2D> api_gen_texture2D(TextureType type, int width,
                                              int height) = 0;
  virtual Scoped<Framebuffer> api_gen_framebuffer(int width, int height) = 0;
  virtual Scoped<Shader> api_gen_shader(const char *directory) = 0;

  // Static access to API functions
  // --------------------------------------------------------------------
  static void set_window_title(const char *title) {
    API->api_set_window_title(title);
  }
  static void set_cursor(CURSOR type) { API->api_set_cursor(type); }
  static void clear_color_buffer(glm::vec4 color) {
    API->api_clear_color_buffer(color);
  }
  static void clear_depth_buffer() { API->api_clear_depth_buffer(); }
  static void resize_window(int width, int height) {
    API->api_resize_window(width, height);
  }
  static void wireframe_mode(bool wireframe) {
    API->api_wireframe_mode(wireframe);
  }
  static void enable_depth_testing(bool enable) {
    API->api_enable_depth_testing(enable);
  }
  static void enable_face_culling(bool enable) {
    API->api_enable_face_culling(enable);
  }
  static void enable_blending(bool enable) { API->api_enable_blending(enable); }
  static glm::vec3 raycast(Camera *camera) { return API->api_raycast(camera); }
  static glm::vec3 raycast(Camera *camera, glm::ivec2 screen_coords) {
    return API->api_raycast(camera, screen_coords);
  }
  static void set_framebuffer(Framebuffer *framebuffer) {
    API->api_set_framebuffer(framebuffer);
  }
  static void dispatch_compute(uint32_t x, uint32_t y, uint32_t z) {
    API->api_dispatch_compute(x, y, z);
  }
  static void render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                 Material *material) {
    API->api_render_simple_mesh(camera, mesh, material);
  }
  static void render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                 Material *material, glm::mat4 &parent_model) {
    API->api_render_simple_mesh(camera, mesh, material, parent_model);
  }
  static void render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                 Material *material, glm::mat4 &parent_model,
                                 unsigned int instance_count,
                                 Buffer<glm::mat4> *models) {
    API->api_render_simple_mesh(camera, mesh, material, parent_model,
                                instance_count, models);
  }
  static void bind_mesh_render_state(SimpleMesh *mesh, Material *material) {
    API->api_bind_mesh_render_state(mesh, material);
  }
  static void destroy_mesh_render_states(SimpleMesh *mesh) {
    API->api_destroy_mesh_render_states(mesh);
  }
  static void push_mesh_geometry_buffer(SimpleMesh *mesh,
                                        Scoped<Buffer<float>> geometry_buffer) {
    API->api_push_mesh_geometry_buffer(mesh, std::move(geometry_buffer));
  }
  static void set_mesh_index_buffer(SimpleMesh *mesh,
                                    Scoped<Buffer<uint32_t>> index_buffer) {
    API->api_set_mesh_index_buffer(mesh, std::move(index_buffer));
  }
  static Scoped<Texture2D> gen_texture2D(const char *image_filepath) {
    return API->api_gen_texture2D(image_filepath);
  }
  static Scoped<Texture2D> gen_texture2D(TextureType type, int width,
                                         int height) {
    return API->api_gen_texture2D(type, width, height);
  }
  static Scoped<Framebuffer> gen_framebuffer(int width, int height) {
    return API->api_gen_framebuffer(width, height);
  }
  static Scoped<Shader> gen_shader(const char *directory) {
    return API->api_gen_shader(directory);
  }

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

  // Scene Template Functions--------------------------------
  // generate single scene and push it onto the stack
  template <typename T, typename... Args> static void gen_scene(Args... args) {
    // generate scene reference of type T and cast to base type
    Referenced<Scene> scn =
        std::dynamic_pointer_cast<Scene>(gen_ref<T>(args...));
    // if T is a scene push the widget onto the stack
    if (scn)
      scenes_.push_back(scn);
  }
  // generate multiple scenes and push them onto the stack
  template <typename T, typename... Args>
  static void gen_scenes(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate scene reference of type T and cast to base type
      Referenced<Scene> scn =
          std::dynamic_pointer_cast<Scene>(gen_ref<T>(args...));
      // if T is a scene push the scene onto the stack
      if (scn)
        scenes_.push_back(scn);
    }
  }
  // get first scene in the stack of a given type
  template <typename T> static T *get_scene() {
    for (auto scn_it = scene_begin(); scn_it != scene_end(); scn_it++) {
      if (auto scene = std::dynamic_pointer_cast<T>(*scn_it)) {
        return scene.get();
      }
    }
    return nullptr;
  }
  // get all scenes of an interface type
  template <typename T> static std::vector<T *> get_scenes() {
    std::vector<T *> scenes{};
    for (auto scn_it = scene_begin(); scn_it != scene_end(); scn_it++) {
      if (auto scene = std::dynamic_pointer_cast<T>(*scn_it)) {
        scenes.push_back(scene.get());
      }
    }
    return scenes;
  }
  // push an existing reference onto the stack
  template <typename T> static void push_scene(Referenced<T> scene) {
    Referenced<Scene> scn = std::dynamic_pointer_cast<Scene>(scene);
    if (scn)
      scenes_.push_back(scn);
  }
  // push a vector of existing references onto the stack
  template <typename T>
  static void push_scenes(std::vector<Referenced<T>> scenes) {
    for (size_t i = 0; i < scenes.size(); i++) {
      Referenced<Scene> scn = std::dynamic_pointer_cast<Scene>(scenes[i]);
      if (scn)
        scenes_.push_back(scn);
    }
  }
  // remove first scene of a given type from the stack and return it
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
  // remove all scenes of a given type from the stack and return them as a
  // vector of references
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

  // Scene stack iterators
  static std::vector<Referenced<Scene>>::iterator scene_begin() {
    return scenes_.begin();
  }
  static std::vector<Referenced<Scene>>::iterator scene_end() {
    return scenes_.end();
  }
  static std::vector<Referenced<Scene>>::reverse_iterator scene_rbegin() {
    return scenes_.rbegin();
  }
  static std::vector<Referenced<Scene>>::reverse_iterator scene_rend() {
    return scenes_.rend();
  }

static void set_renderer(Renderer* renderer)
{
  API = renderer;
}

protected:
  // static variables for renderer
  static RendererInfo info;
  static RendererInput input;
  static bool running;
  // The implemented API
  static Renderer *API;
  // Scene stack
  static std::vector<Referenced<Scene>> scenes_;
};

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