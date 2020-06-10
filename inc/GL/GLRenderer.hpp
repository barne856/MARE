#ifndef GLRENDERER
#define GLRENDERER

// OpenGL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

namespace mare {
namespace opengl {
/**
 * @brief Convert a mare::DrawMethod to the OpenGL GLenum for the draw method.
 *
 * @param draw_method The mare::DrawMethod
 * @return GLenum The OpenGL enum for the draw method.
 */
GLenum GLDrawMethod(DrawMethod draw_method);
} // namespace opengl
/**
 * @brief The OpenGL 4.5 implementation of the Renderer class
 * @details This class can be used to create a new class derived from GLRenderer
 * with specific settings enabled. This derived class is the top level class
 * where the application and Scenes are initialized. A user implementation of
 * GLRenderer can be launched with mare::launch<T>().
 * @see Renderer
 */
class GLRenderer : public Renderer {
public:
  /**
   * @brief This is the OpenGL 4.5 implementation of the main driver function
   * for the Renderer.
   * @details This function will initialize the Rendering API and start the
   * rendering loop.
   */
  void run() final;
  /**
   * @brief This is the OpenGL implementation of the initialization
   * function for the Renderer and will perform any neccesary initialization and
   * window creation.
   * @details Called by GLRenderer::run()
   */
  void init_renderer() final;
  /**
   * @brief This is the OpenGL implementation of the startup
   * function for the Renderer and will start the
   * render loop.
   * @details Called by Renderer::run()
   */
  void start_renderer() final;
  /**
   * @brief OpenGL implementation to set the operating system's clipboad to a
   * string.
   *
   * @param str The string to set the clipboard to.
   */
  virtual void api_set_clipboard_string(std::string str) override;
  /**
   * @brief OpenGL implementation to get a string from the operating system's
   * clipboard if it can be converted to a string.
   *
   * @return The string on the clipboard, empty if the clipbaord cannot be
   * converted to a string or if an error occured.
   */
  virtual std::string api_get_clipboard_string() override;
  /**
   * @brief GLRenderer implementation to set the window title.
   *
   * @param title The title to set.
   * @see Renderer::api_set_window_title(const char*)
   */
  void api_set_window_title(const char *title) override;
  /**
   * @brief GLRenderer implementation to set the cursor.
   *
   * @param type The CursorType to set.
   * @see Renderer::api_set_cursor(CursorType)
   */
  void api_set_cursor(CursorType type) override;
  /**
   * @brief GLRenderer implementation to clear the color buffer.
   *
   * @param color The color to clear the color buffer to.
   * @see Renderer::api_clear_color_buffer(glm::vec4)
   */
  void api_clear_color_buffer(glm::vec4 color) override;
  /**
   * @brief GLRenderer implementation to clear the depth buffer.
   * @details Depth buffer is cleared to 1.0f.
   * @see Renderer::api_clear_depth_buffer()
   */
  void api_clear_depth_buffer() override;
  /**
   * @brief GLRenderer implementation to resize the viewport.
   *
   * @param width The width to resize the viewport to.
   * @param height The height to resize the viewport to.
   * @see Renderer::api_resize_viewport(int, int)
   */
  void api_resize_viewport(int width, int height) override;
  /**
   * @brief GLRenderer implementation to enable wireframe rendering mode.
   *
   * @param wireframe true enables wireframe rendering, false disables wireframe
   * rendering.
   * @see Renderer::api_wireframe_mode(bool)
   */
  void api_wireframe_mode(bool wireframe) override;
  /**
   * @brief GLRenderer implementation to enable depth testing.
   *
   * @param wireframe true enables depth testing, false disables depth testing.
   * @see Renderer::api_enable_depth_testing(bool)
   */
  void api_enable_depth_testing(bool enable) override;
  /**
   * @brief GLRenderer implementation to enable face culling.
   *
   * @param wireframe true enables face culling, false disables face culling.
   * @see Renderer::api_enable_face_culling(bool)
   */
  void api_enable_face_culling(bool enable) override;
  /**
   * @brief GLRenderer implementation to enable blending.
   * @param wireframe true enables blending, false disables blending.
   * @see Renderer::api_enable_blending(bool)
   */
  void api_enable_blending(bool enable) override;
  /**
   * @brief Raycast from the mouse position into the Camera's view of a Scene or
   * Layer.
   * @details The current depth-buffer is used to calculate the 3D point in
   * world space so it must be rendered from the same view as the Camera passed
   * to this function.
   * @param camera The Camera to raycast from.
   * @return The 3D coordinates of the Scene or Layer in world space of the
   * point along the ray intersecting the depth-buffer.
   */
  glm::vec3 api_raycast(Camera *camera) override;
  /**
   * @brief Raycast from screen coordinates using the Camera's view into a
   * Scene or Layer.
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
  glm::vec3 api_raycast(Camera *camera, glm::ivec2 screen_coords) override;
  /**
   * @brief Set a Framebuffer that is different from the default Framebuffer.
   * @details Can be used to render into instead of the default Framebuffer.
   * @param framebuffer The Framebuffer to set.
   */
  virtual void api_set_framebuffer(Framebuffer *framebuffer) override;

  /**
   * @brief GLRenderer implemented function to generate a Texture2D from an
   * image filepath.
   *
   * @param image_filepath The filepath of the image. Can be relative to where
   * the application was executed of absolute.
   * @return A Scoped Texture2D.
   */
  virtual Scoped<Texture2D>
  api_gen_texture2D(const char *image_filepath) override;
  /**
   * @brief GLRenderer implemented function to generate a blank Texture2D for
   * writing to.
   *
   * @param type The TextureType to use.
   * @param width The width in pixels or fragments of the Texture2D.
   * @param height the height in pixels or fragments of the Texture2D.
   * @return A Scoped Texture2D.
   * @see TextureType
   */
  virtual Scoped<Texture2D> api_gen_texture2D(TextureType type, int width,
                                              int height) override;

  /**
   * @brief GLRenderer implemented function to generate a blank Framebuffer to
   * render into.
   *
   * @param width The width in pixels or fragments of the Framebuffer.
   * @param height The height in pixels or fragments of the Framebuffer.
   * @return Scoped<Framebuffer>
   */
  virtual Scoped<Framebuffer> api_gen_framebuffer(int width,
                                                  int height) override;

  /**
   * @brief GLRenderer implemented function to generate a Shader from a
   * directory.
   * @details The directory must contain the glsl files that the shader program
   * consists of with the appropriate extensions.
   *
   * @param directory The directory where the glsl files are stored.
   * @return A Scoped Shader.
   * @see Shader
   */
  virtual Scoped<Shader> api_gen_shader(const char *directory) override;

  /**
   * @brief GLRenderer implemented function to render a SimpleMesh with a
   * Material.
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   */
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material) override;
  /**
   * @brief GLRenderer implemented function to render a SimpleMesh with a
   * Material and a parent Transform.
   *
   * @param camera The Camera to render from.
   * @param mesh The SimpleMesh to render.
   * @param material The Material to render with.
   * @param parent_model The parent Transform to render with.
   */
  virtual void api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                      Material *material,
                                      Transform* parent_transform) override;
  /**
   * @brief GLRenderer implemented function to render a SimpleMesh with a
   * Material and a parent Transform.
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
                                      Buffer<Transform> *models) override;
  /**
   * @brief GLRenderer implemented function to bind a SimpleMesh's render state
   * to a Material.
   *
   * @param mesh The SimpleMesh whose render state will be bound to the
   * Material.
   * @param material The Material the bind to.
   */
  virtual void api_bind_mesh_render_state(SimpleMesh *mesh,
                                          Material *material) override;
  /**
   * @brief GLRenderer implemented function to destroy any render state of a
   * SimpleMesh.
   *
   * @param mesh The SimpleMesh whose render state will be destroyed.
   */
  virtual void api_destroy_mesh_render_states(SimpleMesh *mesh) override;
  /**
   * @brief GLRenderer implemented function to push a Geometry Buffer onto a
   * SimpleMesh's Geometry Buffer stack.
   *
   * @param mesh The SimpleMesh that will have the Geometry Buffer pushed to it.
   * @param geometry_buffer The Geometry Buffer to push.
   */
  virtual void api_push_mesh_geometry_buffer(
      SimpleMesh *mesh, Referenced<Buffer<float>> geometry_buffer) override;
  /**
   * @brief GLRenderer implemented function to set the Index Buffer of a
   * SimpleMesh.
   *
   * @param mesh The SimpleMesh that will have its Index Buffer set.
   * @param index_buffer The Index Buffer to set.
   */
  virtual void
  api_set_mesh_index_buffer(SimpleMesh *mesh,
                            Referenced<Buffer<uint32_t>> index_buffer) override;

  /**
   * @brief Dispatch a compute operation.
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
  virtual void api_dispatch_compute(uint32_t x, uint32_t y,
                                    uint32_t z) override;

private:
  static GLFWwindow *window;

  /**
   * @brief A callback executed whenever OpenGL reports an error.
   * @details See Khronos documentation for
   * glDebugMessageCallback(DEBUGPROC callback, const void * userParam) for more
   * information.
   * @param source The source of the debug message.
   * @param type The type of the debug message.
   * @param id The id of the debuf message.
   * @param severity The severity of the debug message.
   * @param length The length of the debug message.
   * @param message The debug message.
   * @param userParam A user supplied pointer that is passed on each invocation.
   */
  static void GLAPIENTRY debug_message_callback(GLenum source, GLenum type,
                                                GLuint id, GLenum severity,
                                                GLsizei length,
                                                const GLchar *message,
                                                const void *userParam);
  /**
   * @brief Helper function to decode the GLenum source to a string.
   *
   * @param source The source of the OpenGL debug message.
   * @return std::string The decoded string.
   */
  static std::string debug_source_string(GLenum source);
  /**
   * @brief Helper function to decode the GLenum type to a string.
   *
   * @param type The type of the OpenGL debug message.
   * @return std::string The decoded string.
   */
  static std::string debug_type_string(GLenum type);
  /**
   * @brief Helper function to decode the GLenum severity to a string.
   *
   * @param severity The severity of the OpenGL debug message.
   * @return std::string The decoded string.
   */
  static std::string debug_severity_string(GLenum severity);

  /**
   * @brief Static method used by the GLFW library which receives callbacks.
   *
   * @param window A pointer to the window created by GLFW.
   * @param w The width of the new window size.
   * @param h The height of the new window size.
   */
  static void glfw_onResize(GLFWwindow *window, int w, int h);
  /**
   * @brief Static method used by the GLFW library which receives callbacks.
   *
   * @param window A pointer to the window created by GLFW.
   * @param key The GLFW key code of the key.
   * @param scancode The GLFW scancode of the key.
   * @param action The GLFW action of the key.
   * @param mods The GLFW mods of the key.
   */
  static void glfw_onKey(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
  /**
   * @brief Static method used by the GLFW library which receives callbacks.
   *
   * @param window A pointer to the window created by GLFW.
   * @param button The mouse button pressed.
   * @param action The GLFW action of the button.
   * @param mods The GLFW mods of the button.
   */
  static void glfw_onMouseButton(GLFWwindow *window, int button, int action,
                                 int mods);
  /**
   * @brief Static method used by the GLFW library which receives callbacks.
   *
   * @param window A pointer to the window created by GLFW.
   * @param x The x position in window coordinates (pixels from the left of the
   * window) of the mouse.
   * @param y The y position in window coordinates (pixels from the top of the
   * window) of the mouse.
   */
  static void glfw_onMouseMove(GLFWwindow *window, double x, double y);
  /**
   * @brief Static method used by the GLFW library which receives callbacks.
   *
   * @param window A pointer to the window created by GLFW.
   * @param xoffset The x offset since the last callback of the horizontal
   * scroll wheel.
   * @param yoffset The y offset since the last callback of the vertical scroll
   * wheel.
   */
  static void glfw_onMouseWheel(GLFWwindow *window, double xoffset,
                                double yoffset);
  /**
   * @brief Static method used by the GLFW library which receives callbacks from
   * the operating system's text input system.
   *
   * @param window A pointer to the window created by GLFW.
   * @param code_point The unicode code point generated by th operating system.
   */
  static void glfw_onChar(GLFWwindow *window, unsigned int code_point);
  /**
   * @brief Static method used by the GLFW library to report errors.
   *
   * @param error_code The error code for the error
   * @param description A description of the error.
   */
  static void glfw_error_callback(int error_code, const char *description);
  // Cursors
  GLFWcursor *hz_resize_cursor; /**< A GLFW cursor.*/
  GLFWcursor *arrow_cursor;     /**< A GLFW cursor.*/
  GLFWcursor *hand_cursor;      /**< A GLFW cursor.*/
  GLFWcursor *crosshair_cursor; /**< A GLFW cursor.*/
};
} // namespace mare

#endif