// MARE GL
#include "GL/GLRenderer.hpp"
#include "GL/GLBuffers.hpp"
#include "GL/GLShader.hpp"

// MARE
#include "Components/Widget.hpp"
#include "Entity.hpp"
#include "Layer.hpp"
#include "Meshes.hpp"
#include "Scene.hpp"

// Standard Library
#include <iostream>

namespace mare {

GLenum opengl::GLDrawMethod(DrawMethod draw_method) {
  switch (draw_method) {
  case DrawMethod::POINTS:
    return GL_POINTS;
  case DrawMethod::LINES:
    return GL_LINES;
  case DrawMethod::LINE_STRIP:
    return GL_LINE_STRIP;
  case DrawMethod::LINE_LOOP:
    return GL_LINE_LOOP;
  case DrawMethod::TRIANGLES:
    return GL_TRIANGLES;
  case DrawMethod::TRIANGLE_STRIP:
    return GL_TRIANGLE_STRIP;
  case DrawMethod::TRIANGLE_FAN:
    return GL_TRIANGLE_FAN;
  default:
    return GL_POINTS;
  }
}

void GLRenderer::glfw_error_callback(int error_code, const char *description) {
  std::cerr << "GLFW ERROR CODE: [" << error_code << "] " << description
            << std::endl;
}
void GLRenderer::run() {
  init_info();
  info.API = RendererAPI::OpenGL_4_5;
  init_renderer();
  startup();
  start_renderer();
}

void GLRenderer::init_renderer() {
  running = true;

  if (glfwInit() == GLFW_FALSE) {
    std::cerr << "GLFW Failed to initialize." << std::endl;
    running = false;
    return;
  }
  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  if (info.debug_mode.any()) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  }
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, info.samples);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  window =
      glfwCreateWindow(info.window_width, info.window_height, "Untitled",
                       info.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  if (!window) {
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
  glfwSetCharCallback(window, glfw_onChar);
  if (!info.cursor) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }
  hz_resize_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  arrow_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
  crosshair_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  if (info.vsync) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }

  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW failed to initialize." << std::endl;
  }

  if (info.debug_mode.any()) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_message_callback, this);
    std::cout << "GL VENDOR: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "GL VERSION: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GL RENDERER: " << glGetString(GL_RENDERER) << std::endl;
  }
  set_window_title(info.window_title);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  input.mouse_pos = glm::ivec2(xpos, ypos);
}

void GLRenderer::start_renderer() {
  info.current_time = glfwGetTime();
  do {
    double time = glfwGetTime();
    float delta_time = (float)(time - info.current_time);
    // Update render and physics systems
    if (info.scene) {
      info.scene->remove_null_layers();
      info.scene->remove_null_entities();
      info.scene->remove_null_systems();
      info.scene->render(delta_time);
      // Scene/Camera systems
      auto physics_systems = info.scene->get_systems<IPhysicsSystem>();
      auto render_systems = info.scene->get_systems<IRenderSystem>();
      for (auto system : physics_systems) {
        if (system) {
          system->update(delta_time, info.scene);
        }
      }
      for (auto system : render_systems) {
        if (system) {
          system->render(delta_time, info.scene, info.scene);
        }
      }

      // Entities in scene
      for (auto entity_it = info.scene->entity_begin();
           entity_it != info.scene->entity_end(); entity_it++) {
        Entity *entity = entity_it->get();
        if (entity) {
          entity->remove_null_systems();
          physics_systems = entity->get_systems<IPhysicsSystem>();
          render_systems = entity->get_systems<IRenderSystem>();
          for (auto system : physics_systems) {
            if (system) {
              system->update(delta_time, entity);
            }
          }
          for (auto system : render_systems) {
            if (system) {
              system->render(delta_time, info.scene, entity);
            }
          }
        }
      }
      // Layers on scene and entities/widgets in overlays
      for (auto layr_it = info.scene->layer_begin();
           layr_it != info.scene->layer_end(); layr_it++) {
        Layer *layer = layr_it->get();
        if (layer) {
          layer->remove_null_entities();
          layer->remove_null_systems();
          layer->render(delta_time);
          physics_systems = layer->get_systems<IPhysicsSystem>();
          render_systems = layer->get_systems<IRenderSystem>();
          for (auto system : physics_systems) {
            if (system) {
              system->update(delta_time, layer);
            }
          }
          for (auto system : render_systems) {
            if (system) {
              system->render(delta_time, layer, layer);
            }
          }

          for (auto ent_it = layer->entity_begin();
               ent_it != layer->entity_end(); ent_it++) {
            Entity *entity = ent_it->get();
            if (entity) {
              entity->remove_null_systems();
              physics_systems = entity->get_systems<IPhysicsSystem>();
              render_systems = entity->get_systems<IRenderSystem>();
              for (auto system : physics_systems) {
                if (system) {
                  system->update(delta_time, entity);
                }
              }
              for (auto system : render_systems) {
                if (system) {
                  system->render(delta_time, layer, entity);
                }
              }
            }
          }
        }
      }
    }
    info.current_time = time;

    glfwPollEvents();
    glfwSwapBuffers(window);
  } while (running && !glfwWindowShouldClose(window));
  shutdown();
  scenes_.clear();
  glfwDestroyCursor(hz_resize_cursor);
  glfwDestroyCursor(arrow_cursor);
  glfwDestroyCursor(hand_cursor);
  glfwDestroyCursor(crosshair_cursor);
  glfwDestroyWindow(window);
  glfwTerminate();
}

std::string GLRenderer::api_get_vendor_string() {
  std::string result = "GL VENDOR: ";
  result += std::string(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
  return result;
}
std::string GLRenderer::api_get_version_string() {
  std::string result = "GL VERSION: ";
  result +=
      std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  return result;
}
std::string GLRenderer::api_get_renderer_string() {
  std::string result = "GL RENDERER: ";
  result +=
      std::string(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
  return result;
}

void GLRenderer::api_set_clipboard_string(std::string str) {
  glfwSetClipboardString(window, str.c_str());
}
std::string GLRenderer::api_get_clipboard_string() {
  return std::string(glfwGetClipboardString(window));
}

void GLRenderer::api_set_window_title(const char *title) {
  glfwSetWindowTitle(window, title);
}

void GLRenderer::api_set_cursor(CursorType type) {
  switch (type) {
  case CursorType::ARROW:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursor(window, arrow_cursor);
    break;
  case CursorType::HZ_RESIZE:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursor(window, hz_resize_cursor);
    break;
  case CursorType::HAND:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursor(window, hand_cursor);
    break;
  case CursorType::CROSSHAIRS:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursor(window, crosshair_cursor);
    break;
  case CursorType::DISABLED:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    info.cursor = false;
    break;
  case CursorType::ENABLED:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  }
}

void GLRenderer::api_clear_color_buffer(glm::vec4 color) {
  glClearBufferfv(GL_COLOR, 0, &color[0]);
}

void GLRenderer::api_clear_depth_buffer() { glClear(GL_DEPTH_BUFFER_BIT); }

void GLRenderer::api_resize_viewport(int width, int height) {
  info.window_width = width;
  info.window_height = height;
  info.window_aspect = float(info.window_width) / float(info.window_height);
  glViewport(0, 0, width, height);
}

void GLRenderer::api_wireframe_mode(bool wireframe) {
  if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

void GLRenderer::api_enable_depth_testing(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
  } else {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
  }
}

void GLRenderer::api_enable_face_culling(bool enable) {
  if (enable) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void GLRenderer::api_enable_blending(bool enable) {
  if (enable) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  }
}

glm::vec3 GLRenderer::api_raycast(Camera *camera) {
  glm::mat4 inversed_camera =
      glm::inverse(camera->get_projection() * camera->get_view_matrix());
  float x = 2.0f * (float)input.mouse_pos.x / (float)(info.window_width) - 1.0f;
  float y =
      -2.0f * (float)input.mouse_pos.y / (float)(info.window_height) + 1.0f;
  float z = 0.0f;
  glReadPixels(input.mouse_pos.x, info.window_height - input.mouse_pos.y, 1, 1,
               GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  z = 2.0f * z - 1.0f;
  glm::vec4 screen_vector = glm::vec4(x, y, z, 1.0f);
  glm::vec4 world_vector = inversed_camera * screen_vector;
  world_vector /= world_vector.w;
  return glm::vec3(world_vector);
}

glm::vec3 GLRenderer::api_raycast(Camera *camera, glm::ivec2 screen_coords) {
  glm::mat4 inversed_camera =
      glm::inverse(camera->get_projection() * camera->get_view_matrix());
  float x = 2.0f * (float)screen_coords.x / (float)(info.window_width) - 1.0f;
  float y = -2.0f * (float)screen_coords.y / (float)(info.window_height) + 1.0f;
  float z = 0.0f;
  glReadPixels(screen_coords.x, info.window_height - screen_coords.y, 1, 1,
               GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  z = 2.0f * z - 1.0f;
  glm::vec4 screen_vector = glm::vec4(x, y, z, 1.0f);
  glm::vec4 world_vector = inversed_camera * screen_vector;
  world_vector /= world_vector.w;
  return glm::vec3(world_vector);
}

void GLRenderer::api_set_framebuffer(Framebuffer *framebuffer) {
  if (framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->name());
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

// Textures
Scoped<Texture2D> GLRenderer::api_gen_texture2D(const char *image_filepath) {
  return std::make_unique<GLTexture2D>(image_filepath);
}
Scoped<Texture2D> GLRenderer::api_gen_texture2D(TextureType type, int width,
                                                int height) {
  return std::make_unique<GLTexture2D>(type, width, height);
}

// Framebuffers
Scoped<Framebuffer> GLRenderer::api_gen_framebuffer(int width, int height) {
  return std::make_unique<GLFramebuffer>(width, height);
}

// Shaders
Scoped<Shader> GLRenderer::api_gen_shader(const char *directory) {
  return std::make_unique<GLShader>(directory);
}

// normal rendering of simple meshes
void GLRenderer::api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                        Material *material) {
  material->bind();
  mesh->bind(material);
  material->upload_camera(camera);
  material->upload_mesh(mesh, true);
  material->render();
  if (mesh->is_indexed()) {
    glDrawElementsBaseVertex(opengl::GLDrawMethod(mesh->get_draw_method()),
                             GLsizei(mesh->render_count()), GL_UNSIGNED_INT,
                             nullptr, mesh->get_render_index());
  } else {
    glDrawArrays(opengl::GLDrawMethod(mesh->get_draw_method()),
                 mesh->get_render_index(), GLsizei(mesh->render_count()));
  }
  mesh->lock_buffers();
  mesh->swap_buffers();
}
// composite rendering
void GLRenderer::api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                        Material *material,
                                        Transform *parent_model) {
  material->bind();
  mesh->bind(material);
  material->upload_camera(camera);
  material->upload_mesh(mesh, parent_model, true);
  material->render();
  if (mesh->is_indexed()) {
    glDrawElementsBaseVertex(opengl::GLDrawMethod(mesh->get_draw_method()),
                             GLsizei(mesh->render_count()), GL_UNSIGNED_INT,
                             nullptr, mesh->get_render_index());
  } else {
    glDrawArrays(opengl::GLDrawMethod(mesh->get_draw_method()),
                 mesh->get_render_index(), GLsizei(mesh->render_count()));
  }
  mesh->lock_buffers();
  mesh->swap_buffers();
}
// instanced rendering
void GLRenderer::api_render_simple_mesh(Camera *camera, SimpleMesh *mesh,
                                        Material *material,
                                        Transform *parent_model,
                                        unsigned int instance_count,
                                        Buffer<Transform> *models) {
  material->bind();
  mesh->bind(material);
  material->upload_camera(camera);
  material->upload_mesh(mesh, parent_model, models, true);
  material->render();
  if (mesh->is_indexed()) {
    glDrawElementsInstancedBaseVertex(
        opengl::GLDrawMethod(mesh->get_draw_method()),
        static_cast<GLsizei>(mesh->render_count()), GL_UNSIGNED_INT, nullptr,
        instance_count, mesh->get_render_index());
  } else {
    glDrawArraysInstanced(
        opengl::GLDrawMethod(mesh->get_draw_method()), mesh->get_render_index(),
        static_cast<GLsizei>(mesh->render_count()), instance_count);
  }
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
  mesh->lock_buffers();
  mesh->swap_buffers();
}
// Mesh functions
void GLRenderer::api_bind_mesh_render_state(SimpleMesh *mesh,
                                            Material *material) {
  if (!material) {
    glBindVertexArray(0);
    return;
  } else if (mesh->render_states.find(material->name()) ==
             mesh->render_states.end()) {
    uint32_t vertex_array_ID;
    glCreateVertexArrays(1, &vertex_array_ID);
    mesh->render_states.insert({material->name(), vertex_array_ID});
    int buffer_binding_index = 0;
    for (auto &buffer : mesh->geometry_buffers) {
      for (const auto &attrib : buffer->format()) {
        if (attrib.type == AttributeType::POSITION_2D ||
            attrib.type == AttributeType::POSITON_3D ||
            attrib.type == AttributeType::NORMAL ||
            attrib.type == AttributeType::COLOR ||
            attrib.type == AttributeType::TEXTURE_MAP) {
          GLint attrib_loc =
              glGetAttribLocation(material->name(), attrib.name.c_str());
          if (attrib_loc != -1) {
            glEnableVertexArrayAttrib(vertex_array_ID, attrib_loc);
            glVertexArrayAttribFormat(
                vertex_array_ID, attrib_loc,
                static_cast<GLuint>(attrib.component_count()), GL_FLOAT,
                GL_FALSE, static_cast<GLint>(attrib.offset));
            glVertexArrayAttribBinding(
                vertex_array_ID, attrib_loc,
                static_cast<GLuint>(buffer_binding_index));
            glVertexArrayVertexBuffer(
                vertex_array_ID, static_cast<GLuint>(buffer_binding_index),
                static_cast<GLuint>(buffer->name()), 0,
                static_cast<GLsizei>(buffer->format().stride));
          }
        }
      }
      mesh->vertex_render_count = buffer->count();
      buffer_binding_index++;
    }
    if (mesh->index_buffer) {
      glVertexArrayElementBuffer(mesh->render_states[material->name()],
                                 mesh->index_buffer->name());
    }
  }
  glBindVertexArray(mesh->render_states[material->name()]);
}
void GLRenderer::api_destroy_mesh_render_states(SimpleMesh *mesh) {
  for (const auto &[key, value] : mesh->render_states) {
    glDeleteVertexArrays(1, &value);
  }
}
void GLRenderer::api_push_mesh_geometry_buffer(
    SimpleMesh *mesh, Referenced<Buffer<float>> geometry_buffer) {
  mesh->geometry_buffers.push_back(geometry_buffer);
  mesh->invalidate_render_state_cache();
}
void GLRenderer::api_set_mesh_index_buffer(
    SimpleMesh *mesh, Referenced<Buffer<uint32_t>> index_buffer) {
  mesh->index_render_count = index_buffer->count();
  mesh->index_buffer = index_buffer;
  mesh->invalidate_render_state_cache();
}

// Compute Programs
void GLRenderer::api_dispatch_compute(uint32_t x, uint32_t y, uint32_t z) {
  glDispatchCompute(x, y, z);
}

// Debug functions
std::string GLRenderer::debug_source_string(GLenum source) {
  switch (source) {
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
std::string GLRenderer::debug_type_string(GLenum type) {
  switch (type) {
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
std::string GLRenderer::debug_severity_string(GLenum severity) {
  switch (severity) {
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
void GLAPIENTRY GLRenderer::debug_message_callback(GLenum source, GLenum type,
                                                   GLuint id, GLenum severity,
                                                   GLsizei length,
                                                   const GLchar *message,
                                                   const void *userParam) {
  auto debug_state = info.debug_mode;
  if (debug_state[0] && severity == GL_DEBUG_SEVERITY_HIGH) {
    std::cerr << "GL_" << debug_source_string(source) << "_"
              << debug_type_string(type) << ": "
              << debug_severity_string(severity) << " - " << message
              << std::endl;
  }
  if (debug_state[1] && severity == GL_DEBUG_SEVERITY_MEDIUM) {
    std::cerr << "GL_" << debug_source_string(source) << "_"
              << debug_type_string(type) << ": "
              << debug_severity_string(severity) << " - " << message
              << std::endl;
  }
  if (debug_state[2] && severity == GL_DEBUG_SEVERITY_LOW) {
    std::cerr << "GL_" << debug_source_string(source) << "_"
              << debug_type_string(type) << ": "
              << debug_severity_string(severity) << " - " << message
              << std::endl;
  }
  if (debug_state[3] && severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    std::cerr << "GL_" << debug_source_string(source) << "_"
              << debug_type_string(type) << ": "
              << debug_severity_string(severity) << " - " << message
              << std::endl;
  }
}

// Renderer callback functions
void GLRenderer::glfw_onResize(GLFWwindow *window, int w, int h) {
  // callback to the renderer to resize the viewport
  Renderer::API->resize_viewport(w, h);

  if (info.scene) {
    // reverse iterate through layer callbacks first
    bool handled = false;
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      if (layer) {
        // reverse iterate through layer entities
        for (auto ent_it = layer->entity_rbegin();
             ent_it != layer->entity_rend(); ent_it++) {
          Entity *entity = ent_it->get();
          if (entity) {
            auto controls_systems = entity->get_systems<IControlsSystem>();
            // reverse iterate through entity controls callbacks
            for (auto controls_it = controls_systems.rbegin();
                 controls_it != controls_systems.rend(); controls_it++) {
              if (*controls_it) {
                handled = (*controls_it)->on_resize(input, entity);
                if (handled) {
                  return;
                }
              }
            }
          }
        }
        auto controls_systems = layer->get_systems<IControlsSystem>();
        // reverse iterate through layer controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_resize(input, layer);
            if (handled) {
              return;
            }
          }
        }
      }
    }
    // if event is not handled by layers, callback to the scene
    // reverse iterate through scene entities
    for (auto entity_it = info.scene->entity_rbegin();
         entity_it != info.scene->entity_rend(); entity_it++) {
      Entity *entity = entity_it->get();
      if (entity) {
        auto controls_systems = entity->get_systems<IControlsSystem>();
        // reverse iterate through entity controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_resize(input, entity);
            if (handled) {
              return;
            }
          }
        }
      }
    }
    auto controls_systems = info.scene->get_systems<IControlsSystem>();
    // reverse iterate through scene controls callbacks
    for (auto controls_it = controls_systems.rbegin();
         controls_it != controls_systems.rend(); controls_it++) {
      if (*controls_it) {
        handled = (*controls_it)->on_resize(input, info.scene);
        if (handled) {
          return;
        }
      }
    }
  }
}

void GLRenderer::glfw_onKey(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  switch (key) {
  case GLFW_KEY_SPACE:
    if (action == GLFW_PRESS) {
      input.SPACE_PRESSED = true;
      input.SPACE_JUST_PRESSED = true;
      input.SPACE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.SPACE_PRESSED = true;
      input.SPACE_JUST_PRESSED = false;
      input.SPACE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.SPACE_PRESSED = false;
      input.SPACE_JUST_PRESSED = false;
      input.SPACE_JUST_RELEASED = true;
    } else {
      input.SPACE_PRESSED = false;
      input.SPACE_JUST_PRESSED = false;
      input.SPACE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_APOSTROPHE:
    if (action == GLFW_PRESS) {
      input.APOSTROPHE_PRESSED = true;
      input.APOSTROPHE_JUST_PRESSED = true;
      input.APOSTROPHE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.APOSTROPHE_PRESSED = true;
      input.APOSTROPHE_JUST_PRESSED = false;
      input.APOSTROPHE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.APOSTROPHE_PRESSED = false;
      input.APOSTROPHE_JUST_PRESSED = false;
      input.APOSTROPHE_JUST_RELEASED = true;
    } else {
      input.APOSTROPHE_PRESSED = false;
      input.APOSTROPHE_JUST_PRESSED = false;
      input.APOSTROPHE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_COMMA:
    if (action == GLFW_PRESS) {
      input.COMMA_PRESSED = true;
      input.COMMA_JUST_PRESSED = true;
      input.COMMA_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.COMMA_PRESSED = true;
      input.COMMA_JUST_PRESSED = false;
      input.COMMA_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.COMMA_PRESSED = false;
      input.COMMA_JUST_PRESSED = false;
      input.COMMA_JUST_RELEASED = true;
    } else {
      input.COMMA_PRESSED = false;
      input.COMMA_JUST_PRESSED = false;
      input.COMMA_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_MINUS:
    if (action == GLFW_PRESS) {
      input.MINUS_PRESSED = true;
      input.MINUS_JUST_PRESSED = true;
      input.MINUS_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.MINUS_PRESSED = true;
      input.MINUS_JUST_PRESSED = false;
      input.MINUS_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.MINUS_PRESSED = false;
      input.MINUS_JUST_PRESSED = false;
      input.MINUS_JUST_RELEASED = true;
    } else {
      input.MINUS_PRESSED = false;
      input.MINUS_JUST_PRESSED = false;
      input.MINUS_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_PERIOD:
    if (action == GLFW_PRESS) {
      input.PERIOD_PRESSED = true;
      input.PERIOD_JUST_PRESSED = true;
      input.PERIOD_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.PERIOD_PRESSED = true;
      input.PERIOD_JUST_PRESSED = false;
      input.PERIOD_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.PERIOD_PRESSED = false;
      input.PERIOD_JUST_PRESSED = false;
      input.PERIOD_JUST_RELEASED = true;
    } else {
      input.PERIOD_PRESSED = false;
      input.PERIOD_JUST_PRESSED = false;
      input.PERIOD_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_SLASH:
    if (action == GLFW_PRESS) {
      input.FORWARD_SLASH_PRESSED = true;
      input.FORWARD_SLASH_JUST_PRESSED = true;
      input.FORWARD_SLASH_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.FORWARD_SLASH_PRESSED = true;
      input.FORWARD_SLASH_JUST_PRESSED = false;
      input.FORWARD_SLASH_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.FORWARD_SLASH_PRESSED = false;
      input.FORWARD_SLASH_JUST_PRESSED = false;
      input.FORWARD_SLASH_JUST_RELEASED = true;
    } else {
      input.FORWARD_SLASH_PRESSED = false;
      input.FORWARD_SLASH_JUST_PRESSED = false;
      input.FORWARD_SLASH_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_0:
    if (action == GLFW_PRESS) {
      input.ZERO_PRESSED = true;
      input.ZERO_JUST_PRESSED = true;
      input.ZERO_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.ZERO_PRESSED = true;
      input.ZERO_JUST_PRESSED = false;
      input.ZERO_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.ZERO_PRESSED = false;
      input.ZERO_JUST_PRESSED = false;
      input.ZERO_JUST_RELEASED = true;
    } else {
      input.ZERO_PRESSED = false;
      input.ZERO_JUST_PRESSED = false;
      input.ZERO_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_1:
    if (action == GLFW_PRESS) {
      input.ONE_PRESSED = true;
      input.ONE_JUST_PRESSED = true;
      input.ONE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.ONE_PRESSED = true;
      input.ONE_JUST_PRESSED = false;
      input.ONE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.ONE_PRESSED = false;
      input.ONE_JUST_PRESSED = false;
      input.ONE_JUST_RELEASED = true;
    } else {
      input.ONE_PRESSED = false;
      input.ONE_JUST_PRESSED = false;
      input.ONE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_2:
    if (action == GLFW_PRESS) {
      input.TWO_PRESSED = true;
      input.TWO_JUST_PRESSED = true;
      input.TWO_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.TWO_PRESSED = true;
      input.TWO_JUST_PRESSED = false;
      input.TWO_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.TWO_PRESSED = false;
      input.TWO_JUST_PRESSED = false;
      input.TWO_JUST_RELEASED = true;
    } else {
      input.TWO_PRESSED = false;
      input.TWO_JUST_PRESSED = false;
      input.TWO_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_3:
    if (action == GLFW_PRESS) {
      input.THREE_PRESSED = true;
      input.THREE_JUST_PRESSED = true;
      input.THREE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.THREE_PRESSED = true;
      input.THREE_JUST_PRESSED = false;
      input.THREE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.THREE_PRESSED = false;
      input.THREE_JUST_PRESSED = false;
      input.THREE_JUST_RELEASED = true;
    } else {
      input.THREE_PRESSED = false;
      input.THREE_JUST_PRESSED = false;
      input.THREE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_4:
    if (action == GLFW_PRESS) {
      input.FOUR_PRESSED = true;
      input.FOUR_JUST_PRESSED = true;
      input.FOUR_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.FOUR_PRESSED = true;
      input.FOUR_JUST_PRESSED = false;
      input.FOUR_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.FOUR_PRESSED = false;
      input.FOUR_JUST_PRESSED = false;
      input.FOUR_JUST_RELEASED = true;
    } else {
      input.FOUR_PRESSED = false;
      input.FOUR_JUST_PRESSED = false;
      input.FOUR_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_5:
    if (action == GLFW_PRESS) {
      input.FIVE_PRESSED = true;
      input.FIVE_JUST_PRESSED = true;
      input.FIVE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.FIVE_PRESSED = true;
      input.FIVE_JUST_PRESSED = false;
      input.FIVE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.FIVE_PRESSED = false;
      input.FIVE_JUST_PRESSED = false;
      input.FIVE_JUST_RELEASED = true;
    } else {
      input.FIVE_PRESSED = false;
      input.FIVE_JUST_PRESSED = false;
      input.FIVE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_6:
    if (action == GLFW_PRESS) {
      input.SIX_PRESSED = true;
      input.SIX_JUST_PRESSED = true;
      input.SIX_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.SIX_PRESSED = true;
      input.SIX_JUST_PRESSED = false;
      input.SIX_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.SIX_PRESSED = false;
      input.SIX_JUST_PRESSED = false;
      input.SIX_JUST_RELEASED = true;
    } else {
      input.SIX_PRESSED = false;
      input.SIX_JUST_PRESSED = false;
      input.SIX_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_7:
    if (action == GLFW_PRESS) {
      input.SEVEN_PRESSED = true;
      input.SEVEN_JUST_PRESSED = true;
      input.SEVEN_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.SEVEN_PRESSED = true;
      input.SEVEN_JUST_PRESSED = false;
      input.SEVEN_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.SEVEN_PRESSED = false;
      input.SEVEN_JUST_PRESSED = false;
      input.SEVEN_JUST_RELEASED = true;
    } else {
      input.SEVEN_PRESSED = false;
      input.SEVEN_JUST_PRESSED = false;
      input.SEVEN_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_8:
    if (action == GLFW_PRESS) {
      input.EIGHT_PRESSED = true;
      input.EIGHT_JUST_PRESSED = true;
      input.EIGHT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.EIGHT_PRESSED = true;
      input.EIGHT_JUST_PRESSED = false;
      input.EIGHT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.EIGHT_PRESSED = false;
      input.EIGHT_JUST_PRESSED = false;
      input.EIGHT_JUST_RELEASED = true;
    } else {
      input.EIGHT_PRESSED = false;
      input.EIGHT_JUST_PRESSED = false;
      input.EIGHT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_9:
    if (action == GLFW_PRESS) {
      input.NINE_PRESSED = true;
      input.NINE_JUST_PRESSED = true;
      input.NINE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.NINE_PRESSED = true;
      input.NINE_JUST_PRESSED = false;
      input.NINE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.NINE_PRESSED = false;
      input.NINE_JUST_PRESSED = false;
      input.NINE_JUST_RELEASED = true;
    } else {
      input.NINE_PRESSED = false;
      input.NINE_JUST_PRESSED = false;
      input.NINE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_SEMICOLON:
    if (action == GLFW_PRESS) {
      input.SEMICOLON_PRESSED = true;
      input.SEMICOLON_JUST_PRESSED = true;
      input.SEMICOLON_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.SEMICOLON_PRESSED = true;
      input.SEMICOLON_JUST_PRESSED = false;
      input.SEMICOLON_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.SEMICOLON_PRESSED = false;
      input.SEMICOLON_JUST_PRESSED = false;
      input.SEMICOLON_JUST_RELEASED = true;
    } else {
      input.SEMICOLON_PRESSED = false;
      input.SEMICOLON_JUST_PRESSED = false;
      input.SEMICOLON_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_EQUAL:
    if (action == GLFW_PRESS) {
      input.EQUAL_PRESSED = true;
      input.EQUAL_JUST_PRESSED = true;
      input.EQUAL_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.EQUAL_PRESSED = true;
      input.EQUAL_JUST_PRESSED = false;
      input.EQUAL_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.EQUAL_PRESSED = false;
      input.EQUAL_JUST_PRESSED = false;
      input.EQUAL_JUST_RELEASED = true;
    } else {
      input.EQUAL_PRESSED = false;
      input.EQUAL_JUST_PRESSED = false;
      input.EQUAL_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_A:
    if (action == GLFW_PRESS) {
      input.A_PRESSED = true;
      input.A_JUST_PRESSED = true;
      input.A_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.A_PRESSED = true;
      input.A_JUST_PRESSED = false;
      input.A_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.A_PRESSED = false;
      input.A_JUST_PRESSED = false;
      input.A_JUST_RELEASED = true;
    } else {
      input.A_PRESSED = false;
      input.A_JUST_PRESSED = false;
      input.A_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_B:
    if (action == GLFW_PRESS) {
      input.B_PRESSED = true;
      input.B_JUST_PRESSED = true;
      input.B_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.B_PRESSED = true;
      input.B_JUST_PRESSED = false;
      input.B_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.B_PRESSED = false;
      input.B_JUST_PRESSED = false;
      input.B_JUST_RELEASED = true;
    } else {
      input.B_PRESSED = false;
      input.B_JUST_PRESSED = false;
      input.B_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_C:
    if (action == GLFW_PRESS) {
      input.C_PRESSED = true;
      input.C_JUST_PRESSED = true;
      input.C_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.C_PRESSED = true;
      input.C_JUST_PRESSED = false;
      input.C_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.C_PRESSED = false;
      input.C_JUST_PRESSED = false;
      input.C_JUST_RELEASED = true;
    } else {
      input.C_PRESSED = false;
      input.C_JUST_PRESSED = false;
      input.C_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_D:
    if (action == GLFW_PRESS) {
      input.D_PRESSED = true;
      input.D_JUST_PRESSED = true;
      input.D_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.D_PRESSED = true;
      input.D_JUST_PRESSED = false;
      input.D_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.D_PRESSED = false;
      input.D_JUST_PRESSED = false;
      input.D_JUST_RELEASED = true;
    } else {
      input.D_PRESSED = false;
      input.D_JUST_PRESSED = false;
      input.D_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_E:
    if (action == GLFW_PRESS) {
      input.E_PRESSED = true;
      input.E_JUST_PRESSED = true;
      input.E_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.E_PRESSED = true;
      input.E_JUST_PRESSED = false;
      input.E_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.E_PRESSED = false;
      input.E_JUST_PRESSED = false;
      input.E_JUST_RELEASED = true;
    } else {
      input.E_PRESSED = false;
      input.E_JUST_PRESSED = false;
      input.E_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F:
    if (action == GLFW_PRESS) {
      input.F_PRESSED = true;
      input.F_JUST_PRESSED = true;
      input.F_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F_PRESSED = true;
      input.F_JUST_PRESSED = false;
      input.F_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F_PRESSED = false;
      input.F_JUST_PRESSED = false;
      input.F_JUST_RELEASED = true;
    } else {
      input.F_PRESSED = false;
      input.F_JUST_PRESSED = false;
      input.F_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_G:
    if (action == GLFW_PRESS) {
      input.G_PRESSED = true;
      input.G_JUST_PRESSED = true;
      input.G_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.G_PRESSED = true;
      input.G_JUST_PRESSED = false;
      input.G_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.G_PRESSED = false;
      input.G_JUST_PRESSED = false;
      input.G_JUST_RELEASED = true;
    } else {
      input.G_PRESSED = false;
      input.G_JUST_PRESSED = false;
      input.G_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_H:
    if (action == GLFW_PRESS) {
      input.H_PRESSED = true;
      input.H_JUST_PRESSED = true;
      input.H_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.H_PRESSED = true;
      input.H_JUST_PRESSED = false;
      input.H_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.H_PRESSED = false;
      input.H_JUST_PRESSED = false;
      input.H_JUST_RELEASED = true;
    } else {
      input.H_PRESSED = false;
      input.H_JUST_PRESSED = false;
      input.H_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_I:
    if (action == GLFW_PRESS) {
      input.I_PRESSED = true;
      input.I_JUST_PRESSED = true;
      input.I_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.I_PRESSED = true;
      input.I_JUST_PRESSED = false;
      input.I_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.I_PRESSED = false;
      input.I_JUST_PRESSED = false;
      input.I_JUST_RELEASED = true;
    } else {
      input.I_PRESSED = false;
      input.I_JUST_PRESSED = false;
      input.I_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_J:
    if (action == GLFW_PRESS) {
      input.J_PRESSED = true;
      input.J_JUST_PRESSED = true;
      input.J_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.J_PRESSED = true;
      input.J_JUST_PRESSED = false;
      input.J_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.J_PRESSED = false;
      input.J_JUST_PRESSED = false;
      input.J_JUST_RELEASED = true;
    } else {
      input.J_PRESSED = false;
      input.J_JUST_PRESSED = false;
      input.J_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_K:
    if (action == GLFW_PRESS) {
      input.K_PRESSED = true;
      input.K_JUST_PRESSED = true;
      input.K_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.K_PRESSED = true;
      input.K_JUST_PRESSED = false;
      input.K_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.K_PRESSED = false;
      input.K_JUST_PRESSED = false;
      input.K_JUST_RELEASED = true;
    } else {
      input.K_PRESSED = false;
      input.K_JUST_PRESSED = false;
      input.K_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_L:
    if (action == GLFW_PRESS) {
      input.L_PRESSED = true;
      input.L_JUST_PRESSED = true;
      input.L_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.L_PRESSED = true;
      input.L_JUST_PRESSED = false;
      input.L_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.L_PRESSED = false;
      input.L_JUST_PRESSED = false;
      input.L_JUST_RELEASED = true;
    } else {
      input.L_PRESSED = false;
      input.L_JUST_PRESSED = false;
      input.L_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_M:
    if (action == GLFW_PRESS) {
      input.M_PRESSED = true;
      input.M_JUST_PRESSED = true;
      input.M_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.M_PRESSED = true;
      input.M_JUST_PRESSED = false;
      input.M_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.M_PRESSED = false;
      input.M_JUST_PRESSED = false;
      input.M_JUST_RELEASED = true;
    } else {
      input.M_PRESSED = false;
      input.M_JUST_PRESSED = false;
      input.M_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_N:
    if (action == GLFW_PRESS) {
      input.N_PRESSED = true;
      input.N_JUST_PRESSED = true;
      input.N_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.N_PRESSED = true;
      input.N_JUST_PRESSED = false;
      input.N_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.N_PRESSED = false;
      input.N_JUST_PRESSED = false;
      input.N_JUST_RELEASED = true;
    } else {
      input.N_PRESSED = false;
      input.N_JUST_PRESSED = false;
      input.N_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_O:
    if (action == GLFW_PRESS) {
      input.O_PRESSED = true;
      input.O_JUST_PRESSED = true;
      input.O_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.O_PRESSED = true;
      input.O_JUST_PRESSED = false;
      input.O_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.O_PRESSED = false;
      input.O_JUST_PRESSED = false;
      input.O_JUST_RELEASED = true;
    } else {
      input.O_PRESSED = false;
      input.O_JUST_PRESSED = false;
      input.O_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_P:
    if (action == GLFW_PRESS) {
      input.P_PRESSED = true;
      input.P_JUST_PRESSED = true;
      input.P_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.P_PRESSED = true;
      input.P_JUST_PRESSED = false;
      input.P_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.P_PRESSED = false;
      input.P_JUST_PRESSED = false;
      input.P_JUST_RELEASED = true;
    } else {
      input.P_PRESSED = false;
      input.P_JUST_PRESSED = false;
      input.P_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_Q:
    if (action == GLFW_PRESS) {
      input.Q_PRESSED = true;
      input.Q_JUST_PRESSED = true;
      input.Q_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.Q_PRESSED = true;
      input.Q_JUST_PRESSED = false;
      input.Q_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.Q_PRESSED = false;
      input.Q_JUST_PRESSED = false;
      input.Q_JUST_RELEASED = true;
    } else {
      input.Q_PRESSED = false;
      input.Q_JUST_PRESSED = false;
      input.Q_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_R:
    if (action == GLFW_PRESS) {
      input.R_PRESSED = true;
      input.R_JUST_PRESSED = true;
      input.R_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.R_PRESSED = true;
      input.R_JUST_PRESSED = false;
      input.R_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.R_PRESSED = false;
      input.R_JUST_PRESSED = false;
      input.R_JUST_RELEASED = true;
    } else {
      input.R_PRESSED = false;
      input.R_JUST_PRESSED = false;
      input.R_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_S:
    if (action == GLFW_PRESS) {
      input.S_PRESSED = true;
      input.S_JUST_PRESSED = true;
      input.S_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.S_PRESSED = true;
      input.S_JUST_PRESSED = false;
      input.S_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.S_PRESSED = false;
      input.S_JUST_PRESSED = false;
      input.S_JUST_RELEASED = true;
    } else {
      input.S_PRESSED = false;
      input.S_JUST_PRESSED = false;
      input.S_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_T:
    if (action == GLFW_PRESS) {
      input.T_PRESSED = true;
      input.T_JUST_PRESSED = true;
      input.T_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.T_PRESSED = true;
      input.T_JUST_PRESSED = false;
      input.T_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.T_PRESSED = false;
      input.T_JUST_PRESSED = false;
      input.T_JUST_RELEASED = true;
    } else {
      input.T_PRESSED = false;
      input.T_JUST_PRESSED = false;
      input.T_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_U:
    if (action == GLFW_PRESS) {
      input.U_PRESSED = true;
      input.U_JUST_PRESSED = true;
      input.U_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.U_PRESSED = true;
      input.U_JUST_PRESSED = false;
      input.U_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.U_PRESSED = false;
      input.U_JUST_PRESSED = false;
      input.U_JUST_RELEASED = true;
    } else {
      input.U_PRESSED = false;
      input.U_JUST_PRESSED = false;
      input.U_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_V:
    if (action == GLFW_PRESS) {
      input.V_PRESSED = true;
      input.V_JUST_PRESSED = true;
      input.V_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.V_PRESSED = true;
      input.V_JUST_PRESSED = false;
      input.V_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.V_PRESSED = false;
      input.V_JUST_PRESSED = false;
      input.V_JUST_RELEASED = true;
    } else {
      input.V_PRESSED = false;
      input.V_JUST_PRESSED = false;
      input.V_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_W:
    if (action == GLFW_PRESS) {
      input.W_PRESSED = true;
      input.W_JUST_PRESSED = true;
      input.W_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.W_PRESSED = true;
      input.W_JUST_PRESSED = false;
      input.W_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.W_PRESSED = false;
      input.W_JUST_PRESSED = false;
      input.W_JUST_RELEASED = true;
    } else {
      input.W_PRESSED = false;
      input.W_JUST_PRESSED = false;
      input.W_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_X:
    if (action == GLFW_PRESS) {
      input.X_PRESSED = true;
      input.X_JUST_PRESSED = true;
      input.X_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.X_PRESSED = true;
      input.X_JUST_PRESSED = false;
      input.X_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.X_PRESSED = false;
      input.X_JUST_PRESSED = false;
      input.X_JUST_RELEASED = true;
    } else {
      input.X_PRESSED = false;
      input.X_JUST_PRESSED = false;
      input.X_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_Y:
    if (action == GLFW_PRESS) {
      input.Y_PRESSED = true;
      input.Y_JUST_PRESSED = true;
      input.Y_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.Y_PRESSED = true;
      input.Y_JUST_PRESSED = false;
      input.Y_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.Y_PRESSED = false;
      input.Y_JUST_PRESSED = false;
      input.Y_JUST_RELEASED = true;
    } else {
      input.Y_PRESSED = false;
      input.Y_JUST_PRESSED = false;
      input.Y_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_Z:
    if (action == GLFW_PRESS) {
      input.Z_PRESSED = true;
      input.Z_JUST_PRESSED = true;
      input.Z_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.Z_PRESSED = true;
      input.Z_JUST_PRESSED = false;
      input.Z_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.Z_PRESSED = false;
      input.Z_JUST_PRESSED = false;
      input.Z_JUST_RELEASED = true;
    } else {
      input.Z_PRESSED = false;
      input.Z_JUST_PRESSED = false;
      input.Z_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_LEFT_BRACKET:
    if (action == GLFW_PRESS) {
      input.LEFT_BRACKET_PRESSED = true;
      input.LEFT_BRACKET_JUST_PRESSED = true;
      input.LEFT_BRACKET_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_BRACKET_PRESSED = true;
      input.LEFT_BRACKET_JUST_PRESSED = false;
      input.LEFT_BRACKET_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_BRACKET_PRESSED = false;
      input.LEFT_BRACKET_JUST_PRESSED = false;
      input.LEFT_BRACKET_JUST_RELEASED = true;
    } else {
      input.LEFT_BRACKET_PRESSED = false;
      input.LEFT_BRACKET_JUST_PRESSED = false;
      input.LEFT_BRACKET_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_RIGHT_BRACKET:
    if (action == GLFW_PRESS) {
      input.RIGHT_BRACKET_PRESSED = true;
      input.RIGHT_BRACKET_JUST_PRESSED = true;
      input.RIGHT_BRACKET_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_BRACKET_PRESSED = true;
      input.RIGHT_BRACKET_JUST_PRESSED = false;
      input.RIGHT_BRACKET_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_BRACKET_PRESSED = false;
      input.RIGHT_BRACKET_JUST_PRESSED = false;
      input.RIGHT_BRACKET_JUST_RELEASED = true;
    } else {
      input.RIGHT_BRACKET_PRESSED = false;
      input.RIGHT_BRACKET_JUST_PRESSED = false;
      input.RIGHT_BRACKET_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_GRAVE_ACCENT:
    if (action == GLFW_PRESS) {
      input.GRAVE_ACCENT_PRESSED = true;
      input.GRAVE_ACCENT_JUST_PRESSED = true;
      input.GRAVE_ACCENT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.GRAVE_ACCENT_PRESSED = true;
      input.GRAVE_ACCENT_JUST_PRESSED = false;
      input.GRAVE_ACCENT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.GRAVE_ACCENT_PRESSED = false;
      input.GRAVE_ACCENT_JUST_PRESSED = false;
      input.GRAVE_ACCENT_JUST_RELEASED = true;
    } else {
      input.GRAVE_ACCENT_PRESSED = false;
      input.GRAVE_ACCENT_JUST_PRESSED = false;
      input.GRAVE_ACCENT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_ESCAPE:
    if (action == GLFW_PRESS) {
      input.ESCAPE_PRESSED = true;
      input.ESCAPE_JUST_PRESSED = true;
      input.ESCAPE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.ESCAPE_PRESSED = true;
      input.ESCAPE_JUST_PRESSED = false;
      input.ESCAPE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.ESCAPE_PRESSED = false;
      input.ESCAPE_JUST_PRESSED = false;
      input.ESCAPE_JUST_RELEASED = true;
    } else {
      input.ESCAPE_PRESSED = false;
      input.ESCAPE_JUST_PRESSED = false;
      input.ESCAPE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_ENTER:
    if (action == GLFW_PRESS) {
      input.ENTER_PRESSED = true;
      input.ENTER_JUST_PRESSED = true;
      input.ENTER_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.ENTER_PRESSED = true;
      input.ENTER_JUST_PRESSED = false;
      input.ENTER_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.ENTER_PRESSED = false;
      input.ENTER_JUST_PRESSED = false;
      input.ENTER_JUST_RELEASED = true;
    } else {
      input.ENTER_PRESSED = false;
      input.ENTER_JUST_PRESSED = false;
      input.ENTER_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_TAB:
    if (action == GLFW_PRESS) {
      input.TAB_PRESSED = true;
      input.TAB_JUST_PRESSED = true;
      input.TAB_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.TAB_PRESSED = true;
      input.TAB_JUST_PRESSED = false;
      input.TAB_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.TAB_PRESSED = false;
      input.TAB_JUST_PRESSED = false;
      input.TAB_JUST_RELEASED = true;
    } else {
      input.TAB_PRESSED = false;
      input.TAB_JUST_PRESSED = false;
      input.TAB_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_BACKSPACE:
    if (action == GLFW_PRESS) {
      input.BACKSPACE_PRESSED = true;
      input.BACKSPACE_JUST_PRESSED = true;
      input.BACKSPACE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.BACKSPACE_PRESSED = true;
      input.BACKSPACE_JUST_PRESSED = false;
      input.BACKSPACE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.BACKSPACE_PRESSED = false;
      input.BACKSPACE_JUST_PRESSED = false;
      input.BACKSPACE_JUST_RELEASED = true;
    } else {
      input.BACKSPACE_PRESSED = false;
      input.BACKSPACE_JUST_PRESSED = false;
      input.BACKSPACE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_INSERT:
    if (action == GLFW_PRESS) {
      input.INSERT_PRESSED = true;
      input.INSERT_JUST_PRESSED = true;
      input.INSERT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.INSERT_PRESSED = true;
      input.INSERT_JUST_PRESSED = false;
      input.INSERT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.INSERT_PRESSED = false;
      input.INSERT_JUST_PRESSED = false;
      input.INSERT_JUST_RELEASED = true;
    } else {
      input.INSERT_PRESSED = false;
      input.INSERT_JUST_PRESSED = false;
      input.INSERT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_DELETE:
    if (action == GLFW_PRESS) {
      input.DELETE_PRESSED = true;
      input.DELETE_JUST_PRESSED = true;
      input.DELETE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.DELETE_PRESSED = true;
      input.DELETE_JUST_PRESSED = false;
      input.DELETE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.DELETE_PRESSED = false;
      input.DELETE_JUST_PRESSED = false;
      input.DELETE_JUST_RELEASED = true;
    } else {
      input.DELETE_PRESSED = false;
      input.DELETE_JUST_PRESSED = false;
      input.DELETE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_RIGHT:
    if (action == GLFW_PRESS) {
      input.RIGHT_PRESSED = true;
      input.RIGHT_JUST_PRESSED = true;
      input.RIGHT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_PRESSED = true;
      input.RIGHT_JUST_PRESSED = false;
      input.RIGHT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_PRESSED = false;
      input.RIGHT_JUST_PRESSED = false;
      input.RIGHT_JUST_RELEASED = true;
    } else {
      input.RIGHT_PRESSED = false;
      input.RIGHT_JUST_PRESSED = false;
      input.RIGHT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_LEFT:
    if (action == GLFW_PRESS) {
      input.LEFT_PRESSED = true;
      input.LEFT_JUST_PRESSED = true;
      input.LEFT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_PRESSED = true;
      input.LEFT_JUST_PRESSED = false;
      input.LEFT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_PRESSED = false;
      input.LEFT_JUST_PRESSED = false;
      input.LEFT_JUST_RELEASED = true;
    } else {
      input.LEFT_PRESSED = false;
      input.LEFT_JUST_PRESSED = false;
      input.LEFT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_UP:
    if (action == GLFW_PRESS) {
      input.UP_PRESSED = true;
      input.UP_JUST_PRESSED = true;
      input.UP_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.UP_PRESSED = true;
      input.UP_JUST_PRESSED = false;
      input.UP_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.UP_PRESSED = false;
      input.UP_JUST_PRESSED = false;
      input.UP_JUST_RELEASED = true;
    } else {
      input.UP_PRESSED = false;
      input.UP_JUST_PRESSED = false;
      input.UP_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_DOWN:
    if (action == GLFW_PRESS) {
      input.DOWN_PRESSED = true;
      input.DOWN_JUST_PRESSED = true;
      input.DOWN_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.DOWN_PRESSED = true;
      input.DOWN_JUST_PRESSED = false;
      input.DOWN_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.DOWN_PRESSED = false;
      input.DOWN_JUST_PRESSED = false;
      input.DOWN_JUST_RELEASED = true;
    } else {
      input.DOWN_PRESSED = false;
      input.DOWN_JUST_PRESSED = false;
      input.DOWN_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_PAGE_UP:
    if (action == GLFW_PRESS) {
      input.PAGE_UP_PRESSED = true;
      input.PAGE_UP_JUST_PRESSED = true;
      input.PAGE_UP_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.PAGE_UP_PRESSED = true;
      input.PAGE_UP_JUST_PRESSED = false;
      input.PAGE_UP_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.PAGE_UP_PRESSED = false;
      input.PAGE_UP_JUST_PRESSED = false;
      input.PAGE_UP_JUST_RELEASED = true;
    } else {
      input.PAGE_UP_PRESSED = false;
      input.PAGE_UP_JUST_PRESSED = false;
      input.PAGE_UP_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_PAGE_DOWN:
    if (action == GLFW_PRESS) {
      input.PAGE_DOWN_PRESSED = true;
      input.PAGE_DOWN_JUST_PRESSED = true;
      input.PAGE_DOWN_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.PAGE_DOWN_PRESSED = true;
      input.PAGE_DOWN_JUST_PRESSED = false;
      input.PAGE_DOWN_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.PAGE_DOWN_PRESSED = false;
      input.PAGE_DOWN_JUST_PRESSED = false;
      input.PAGE_DOWN_JUST_RELEASED = true;
    } else {
      input.PAGE_DOWN_PRESSED = false;
      input.PAGE_DOWN_JUST_PRESSED = false;
      input.PAGE_DOWN_JUST_RELEASED = true;
    }
    break;
    if (action == GLFW_PRESS) {
      input.HOME_PRESSED = true;
      input.HOME_JUST_PRESSED = true;
      input.HOME_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.HOME_PRESSED = true;
      input.HOME_JUST_PRESSED = false;
      input.HOME_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.HOME_PRESSED = false;
      input.HOME_JUST_PRESSED = false;
      input.HOME_JUST_RELEASED = true;
    } else {
      input.HOME_PRESSED = false;
      input.HOME_JUST_PRESSED = false;
      input.HOME_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_END:
    if (action == GLFW_PRESS) {
      input.END_PRESSED = true;
      input.END_JUST_PRESSED = true;
      input.END_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.END_PRESSED = true;
      input.END_JUST_PRESSED = false;
      input.END_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.END_PRESSED = false;
      input.END_JUST_PRESSED = false;
      input.END_JUST_RELEASED = true;
    } else {
      input.END_PRESSED = false;
      input.END_JUST_PRESSED = false;
      input.END_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_CAPS_LOCK:
    if (action == GLFW_PRESS) {
      input.CAPS_LOCK_PRESSED = true;
      input.CAPS_LOCK_JUST_PRESSED = true;
      input.CAPS_LOCK_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.CAPS_LOCK_PRESSED = true;
      input.CAPS_LOCK_JUST_PRESSED = false;
      input.CAPS_LOCK_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.CAPS_LOCK_PRESSED = false;
      input.CAPS_LOCK_JUST_PRESSED = false;
      input.CAPS_LOCK_JUST_RELEASED = true;
    } else {
      input.CAPS_LOCK_PRESSED = false;
      input.CAPS_LOCK_JUST_PRESSED = false;
      input.CAPS_LOCK_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_SCROLL_LOCK:
    if (action == GLFW_PRESS) {
      input.SCROLL_LOCK_PRESSED = true;
      input.SCROLL_LOCK_JUST_PRESSED = true;
      input.SCROLL_LOCK_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.SCROLL_LOCK_PRESSED = true;
      input.SCROLL_LOCK_JUST_PRESSED = false;
      input.SCROLL_LOCK_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.SCROLL_LOCK_PRESSED = false;
      input.SCROLL_LOCK_JUST_PRESSED = false;
      input.SCROLL_LOCK_JUST_RELEASED = true;
    } else {
      input.SCROLL_LOCK_PRESSED = false;
      input.SCROLL_LOCK_JUST_PRESSED = false;
      input.SCROLL_LOCK_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_NUM_LOCK:
    if (action == GLFW_PRESS) {
      input.NUM_LOCK_PRESSED = true;
      input.NUM_LOCK_JUST_PRESSED = true;
      input.NUM_LOCK_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.NUM_LOCK_PRESSED = true;
      input.NUM_LOCK_JUST_PRESSED = false;
      input.NUM_LOCK_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.NUM_LOCK_PRESSED = false;
      input.NUM_LOCK_JUST_PRESSED = false;
      input.NUM_LOCK_JUST_RELEASED = true;
    } else {
      input.NUM_LOCK_PRESSED = false;
      input.NUM_LOCK_JUST_PRESSED = false;
      input.NUM_LOCK_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_PRINT_SCREEN:
    if (action == GLFW_PRESS) {
      input.PRINT_SCREEN_PRESSED = true;
      input.PRINT_SCREEN_JUST_PRESSED = true;
      input.PRINT_SCREEN_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.PRINT_SCREEN_PRESSED = true;
      input.PRINT_SCREEN_JUST_PRESSED = false;
      input.PRINT_SCREEN_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.PRINT_SCREEN_PRESSED = false;
      input.PRINT_SCREEN_JUST_PRESSED = false;
      input.PRINT_SCREEN_JUST_RELEASED = true;
    } else {
      input.PRINT_SCREEN_PRESSED = false;
      input.PRINT_SCREEN_JUST_PRESSED = false;
      input.PRINT_SCREEN_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_PAUSE:
    if (action == GLFW_PRESS) {
      input.PAUSE_PRESSED = true;
      input.PAUSE_JUST_PRESSED = true;
      input.PAUSE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.PAUSE_PRESSED = true;
      input.PAUSE_JUST_PRESSED = false;
      input.PAUSE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.PAUSE_PRESSED = false;
      input.PAUSE_JUST_PRESSED = false;
      input.PAUSE_JUST_RELEASED = true;
    } else {
      input.PAUSE_PRESSED = false;
      input.PAUSE_JUST_PRESSED = false;
      input.PAUSE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F1:
    if (action == GLFW_PRESS) {
      input.F1_PRESSED = true;
      input.F1_JUST_PRESSED = true;
      input.F1_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F1_PRESSED = true;
      input.F1_JUST_PRESSED = false;
      input.F1_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F1_PRESSED = false;
      input.F1_JUST_PRESSED = false;
      input.F1_JUST_RELEASED = true;
    } else {
      input.F1_PRESSED = false;
      input.F1_JUST_PRESSED = false;
      input.F1_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F2:
    if (action == GLFW_PRESS) {
      input.F2_PRESSED = true;
      input.F2_JUST_PRESSED = true;
      input.F2_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F2_PRESSED = true;
      input.F2_JUST_PRESSED = false;
      input.F2_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F2_PRESSED = false;
      input.F2_JUST_PRESSED = false;
      input.F2_JUST_RELEASED = true;
    } else {
      input.F2_PRESSED = false;
      input.F2_JUST_PRESSED = false;
      input.F2_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F3:
    if (action == GLFW_PRESS) {
      input.F3_PRESSED = true;
      input.F3_JUST_PRESSED = true;
      input.F3_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F3_PRESSED = true;
      input.F3_JUST_PRESSED = false;
      input.F3_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F3_PRESSED = false;
      input.F3_JUST_PRESSED = false;
      input.F3_JUST_RELEASED = true;
    } else {
      input.F3_PRESSED = false;
      input.F3_JUST_PRESSED = false;
      input.F3_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F4:
    if (action == GLFW_PRESS) {
      input.F4_PRESSED = true;
      input.F4_JUST_PRESSED = true;
      input.F4_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F4_PRESSED = true;
      input.F4_JUST_PRESSED = false;
      input.F4_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F4_PRESSED = false;
      input.F4_JUST_PRESSED = false;
      input.F4_JUST_RELEASED = true;
    } else {
      input.F4_PRESSED = false;
      input.F4_JUST_PRESSED = false;
      input.F4_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F5:
    if (action == GLFW_PRESS) {
      input.F5_PRESSED = true;
      input.F5_JUST_PRESSED = true;
      input.F5_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F5_PRESSED = true;
      input.F5_JUST_PRESSED = false;
      input.F5_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F5_PRESSED = false;
      input.F5_JUST_PRESSED = false;
      input.F5_JUST_RELEASED = true;
    } else {
      input.F5_PRESSED = false;
      input.F5_JUST_PRESSED = false;
      input.F5_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F6:
    if (action == GLFW_PRESS) {
      input.F6_PRESSED = true;
      input.F6_JUST_PRESSED = true;
      input.F6_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F6_PRESSED = true;
      input.F6_JUST_PRESSED = false;
      input.F6_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F6_PRESSED = false;
      input.F6_JUST_PRESSED = false;
      input.F6_JUST_RELEASED = true;
    } else {
      input.F6_PRESSED = false;
      input.F6_JUST_PRESSED = false;
      input.F6_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F7:
    if (action == GLFW_PRESS) {
      input.F7_PRESSED = true;
      input.F7_JUST_PRESSED = true;
      input.F7_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F7_PRESSED = true;
      input.F7_JUST_PRESSED = false;
      input.F7_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F7_PRESSED = false;
      input.F7_JUST_PRESSED = false;
      input.F7_JUST_RELEASED = true;
    } else {
      input.F7_PRESSED = false;
      input.F7_JUST_PRESSED = false;
      input.F7_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F8:
    if (action == GLFW_PRESS) {
      input.F8_PRESSED = true;
      input.F8_JUST_PRESSED = true;
      input.F8_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F8_PRESSED = true;
      input.F8_JUST_PRESSED = false;
      input.F8_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F8_PRESSED = false;
      input.F8_JUST_PRESSED = false;
      input.F8_JUST_RELEASED = true;
    } else {
      input.F8_PRESSED = false;
      input.F8_JUST_PRESSED = false;
      input.F8_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F9:
    if (action == GLFW_PRESS) {
      input.F9_PRESSED = true;
      input.F9_JUST_PRESSED = true;
      input.F9_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F9_PRESSED = true;
      input.F9_JUST_PRESSED = false;
      input.F9_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F9_PRESSED = false;
      input.F9_JUST_PRESSED = false;
      input.F9_JUST_RELEASED = true;
    } else {
      input.F9_PRESSED = false;
      input.F9_JUST_PRESSED = false;
      input.F9_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F10:
    if (action == GLFW_PRESS) {
      input.F10_PRESSED = true;
      input.F10_JUST_PRESSED = true;
      input.F10_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F10_PRESSED = true;
      input.F10_JUST_PRESSED = false;
      input.F10_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F10_PRESSED = false;
      input.F10_JUST_PRESSED = false;
      input.F10_JUST_RELEASED = true;
    } else {
      input.F10_PRESSED = false;
      input.F10_JUST_PRESSED = false;
      input.F10_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F11:
    if (action == GLFW_PRESS) {
      input.F11_PRESSED = true;
      input.F11_JUST_PRESSED = true;
      input.F11_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F11_PRESSED = true;
      input.F11_JUST_PRESSED = false;
      input.F11_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F11_PRESSED = false;
      input.F11_JUST_PRESSED = false;
      input.F11_JUST_RELEASED = true;
    } else {
      input.F11_PRESSED = false;
      input.F11_JUST_PRESSED = false;
      input.F11_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_F12:
    if (action == GLFW_PRESS) {
      input.F12_PRESSED = true;
      input.F12_JUST_PRESSED = true;
      input.F12_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.F12_PRESSED = true;
      input.F12_JUST_PRESSED = false;
      input.F12_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.F12_PRESSED = false;
      input.F12_JUST_PRESSED = false;
      input.F12_JUST_RELEASED = true;
    } else {
      input.F12_PRESSED = false;
      input.F12_JUST_PRESSED = false;
      input.F12_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_0:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_0_PRESSED = true;
      input.KEY_PAD_0_JUST_PRESSED = true;
      input.KEY_PAD_0_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_0_PRESSED = true;
      input.KEY_PAD_0_JUST_PRESSED = false;
      input.KEY_PAD_0_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_0_PRESSED = false;
      input.KEY_PAD_0_JUST_PRESSED = false;
      input.KEY_PAD_0_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_0_PRESSED = false;
      input.KEY_PAD_0_JUST_PRESSED = false;
      input.KEY_PAD_0_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_1:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_1_PRESSED = true;
      input.KEY_PAD_1_JUST_PRESSED = true;
      input.KEY_PAD_1_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_1_PRESSED = true;
      input.KEY_PAD_1_JUST_PRESSED = false;
      input.KEY_PAD_1_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_1_PRESSED = false;
      input.KEY_PAD_1_JUST_PRESSED = false;
      input.KEY_PAD_1_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_1_PRESSED = false;
      input.KEY_PAD_1_JUST_PRESSED = false;
      input.KEY_PAD_1_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_2:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_2_PRESSED = true;
      input.KEY_PAD_2_JUST_PRESSED = true;
      input.KEY_PAD_2_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_2_PRESSED = true;
      input.KEY_PAD_2_JUST_PRESSED = false;
      input.KEY_PAD_2_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_2_PRESSED = false;
      input.KEY_PAD_2_JUST_PRESSED = false;
      input.KEY_PAD_2_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_2_PRESSED = false;
      input.KEY_PAD_2_JUST_PRESSED = false;
      input.KEY_PAD_2_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_3:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_3_PRESSED = true;
      input.KEY_PAD_3_JUST_PRESSED = true;
      input.KEY_PAD_3_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_3_PRESSED = true;
      input.KEY_PAD_3_JUST_PRESSED = false;
      input.KEY_PAD_3_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_3_PRESSED = false;
      input.KEY_PAD_3_JUST_PRESSED = false;
      input.KEY_PAD_3_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_3_PRESSED = false;
      input.KEY_PAD_3_JUST_PRESSED = false;
      input.KEY_PAD_3_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_4:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_4_PRESSED = true;
      input.KEY_PAD_4_JUST_PRESSED = true;
      input.KEY_PAD_4_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_4_PRESSED = true;
      input.KEY_PAD_4_JUST_PRESSED = false;
      input.KEY_PAD_4_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_4_PRESSED = false;
      input.KEY_PAD_4_JUST_PRESSED = false;
      input.KEY_PAD_4_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_4_PRESSED = false;
      input.KEY_PAD_4_JUST_PRESSED = false;
      input.KEY_PAD_4_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_5:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_5_PRESSED = true;
      input.KEY_PAD_5_JUST_PRESSED = true;
      input.KEY_PAD_5_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_5_PRESSED = true;
      input.KEY_PAD_5_JUST_PRESSED = false;
      input.KEY_PAD_5_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_5_PRESSED = false;
      input.KEY_PAD_5_JUST_PRESSED = false;
      input.KEY_PAD_5_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_5_PRESSED = false;
      input.KEY_PAD_5_JUST_PRESSED = false;
      input.KEY_PAD_5_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_6:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_6_PRESSED = true;
      input.KEY_PAD_6_JUST_PRESSED = true;
      input.KEY_PAD_6_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_6_PRESSED = true;
      input.KEY_PAD_6_JUST_PRESSED = false;
      input.KEY_PAD_6_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_6_PRESSED = false;
      input.KEY_PAD_6_JUST_PRESSED = false;
      input.KEY_PAD_6_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_6_PRESSED = false;
      input.KEY_PAD_6_JUST_PRESSED = false;
      input.KEY_PAD_6_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_7:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_7_PRESSED = true;
      input.KEY_PAD_7_JUST_PRESSED = true;
      input.KEY_PAD_7_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_7_PRESSED = true;
      input.KEY_PAD_7_JUST_PRESSED = false;
      input.KEY_PAD_7_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_7_PRESSED = false;
      input.KEY_PAD_7_JUST_PRESSED = false;
      input.KEY_PAD_7_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_7_PRESSED = false;
      input.KEY_PAD_7_JUST_PRESSED = false;
      input.KEY_PAD_7_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_8:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_8_PRESSED = true;
      input.KEY_PAD_8_JUST_PRESSED = true;
      input.KEY_PAD_8_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_8_PRESSED = true;
      input.KEY_PAD_8_JUST_PRESSED = false;
      input.KEY_PAD_8_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_8_PRESSED = false;
      input.KEY_PAD_8_JUST_PRESSED = false;
      input.KEY_PAD_8_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_8_PRESSED = false;
      input.KEY_PAD_8_JUST_PRESSED = false;
      input.KEY_PAD_8_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_9:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_9_PRESSED = true;
      input.KEY_PAD_9_JUST_PRESSED = true;
      input.KEY_PAD_9_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_9_PRESSED = true;
      input.KEY_PAD_9_JUST_PRESSED = false;
      input.KEY_PAD_9_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_9_PRESSED = false;
      input.KEY_PAD_9_JUST_PRESSED = false;
      input.KEY_PAD_9_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_9_PRESSED = false;
      input.KEY_PAD_9_JUST_PRESSED = false;
      input.KEY_PAD_9_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_DECIMAL:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_DECIMAL_PRESSED = true;
      input.KEY_PAD_DECIMAL_JUST_PRESSED = true;
      input.KEY_PAD_DECIMAL_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_DECIMAL_PRESSED = true;
      input.KEY_PAD_DECIMAL_JUST_PRESSED = false;
      input.KEY_PAD_DECIMAL_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_DECIMAL_PRESSED = false;
      input.KEY_PAD_DECIMAL_JUST_PRESSED = false;
      input.KEY_PAD_DECIMAL_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_DECIMAL_PRESSED = false;
      input.KEY_PAD_DECIMAL_JUST_PRESSED = false;
      input.KEY_PAD_DECIMAL_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_DIVIDE:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_DIVIDE_PRESSED = true;
      input.KEY_PAD_DIVIDE_JUST_PRESSED = true;
      input.KEY_PAD_DIVIDE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_DIVIDE_PRESSED = true;
      input.KEY_PAD_DIVIDE_JUST_PRESSED = false;
      input.KEY_PAD_DIVIDE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_DIVIDE_PRESSED = false;
      input.KEY_PAD_DIVIDE_JUST_PRESSED = false;
      input.KEY_PAD_DIVIDE_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_DIVIDE_PRESSED = false;
      input.KEY_PAD_DIVIDE_JUST_PRESSED = false;
      input.KEY_PAD_DIVIDE_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_MULTIPLY:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_MULTIPLY_PRESSED = true;
      input.KEY_PAD_MULTIPLY_JUST_PRESSED = true;
      input.KEY_PAD_MULTIPLY_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_MULTIPLY_PRESSED = true;
      input.KEY_PAD_MULTIPLY_JUST_PRESSED = false;
      input.KEY_PAD_MULTIPLY_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_MULTIPLY_PRESSED = false;
      input.KEY_PAD_MULTIPLY_JUST_PRESSED = false;
      input.KEY_PAD_MULTIPLY_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_MULTIPLY_PRESSED = false;
      input.KEY_PAD_MULTIPLY_JUST_PRESSED = false;
      input.KEY_PAD_MULTIPLY_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_SUBTRACT:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_SUBTRACT_PRESSED = true;
      input.KEY_PAD_SUBTRACT_JUST_PRESSED = true;
      input.KEY_PAD_SUBTRACT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_SUBTRACT_PRESSED = true;
      input.KEY_PAD_SUBTRACT_JUST_PRESSED = false;
      input.KEY_PAD_SUBTRACT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_SUBTRACT_PRESSED = false;
      input.KEY_PAD_SUBTRACT_JUST_PRESSED = false;
      input.KEY_PAD_SUBTRACT_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_SUBTRACT_PRESSED = false;
      input.KEY_PAD_SUBTRACT_JUST_PRESSED = false;
      input.KEY_PAD_SUBTRACT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_ADD:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_ADD_PRESSED = true;
      input.KEY_PAD_ADD_JUST_PRESSED = true;
      input.KEY_PAD_ADD_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_ADD_PRESSED = true;
      input.KEY_PAD_ADD_JUST_PRESSED = false;
      input.KEY_PAD_ADD_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_ADD_PRESSED = false;
      input.KEY_PAD_ADD_JUST_PRESSED = false;
      input.KEY_PAD_ADD_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_ADD_PRESSED = false;
      input.KEY_PAD_ADD_JUST_PRESSED = false;
      input.KEY_PAD_ADD_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_ENTER:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_ENTER_PRESSED = true;
      input.KEY_PAD_ENTER_JUST_PRESSED = true;
      input.KEY_PAD_ENTER_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_ENTER_PRESSED = true;
      input.KEY_PAD_ENTER_JUST_PRESSED = false;
      input.KEY_PAD_ENTER_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_ENTER_PRESSED = false;
      input.KEY_PAD_ENTER_JUST_PRESSED = false;
      input.KEY_PAD_ENTER_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_ENTER_PRESSED = false;
      input.KEY_PAD_ENTER_JUST_PRESSED = false;
      input.KEY_PAD_ENTER_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_KP_EQUAL:
    if (action == GLFW_PRESS) {
      input.KEY_PAD_EQUAL_PRESSED = true;
      input.KEY_PAD_EQUAL_JUST_PRESSED = true;
      input.KEY_PAD_EQUAL_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.KEY_PAD_EQUAL_PRESSED = true;
      input.KEY_PAD_EQUAL_JUST_PRESSED = false;
      input.KEY_PAD_EQUAL_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.KEY_PAD_EQUAL_PRESSED = false;
      input.KEY_PAD_EQUAL_JUST_PRESSED = false;
      input.KEY_PAD_EQUAL_JUST_RELEASED = true;
    } else {
      input.KEY_PAD_EQUAL_PRESSED = false;
      input.KEY_PAD_EQUAL_JUST_PRESSED = false;
      input.KEY_PAD_EQUAL_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_LEFT_SHIFT:
    if (action == GLFW_PRESS) {
      input.LEFT_SHIFT_PRESSED = true;
      input.LEFT_SHIFT_JUST_PRESSED = true;
      input.LEFT_SHIFT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_SHIFT_PRESSED = true;
      input.LEFT_SHIFT_JUST_PRESSED = false;
      input.LEFT_SHIFT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_SHIFT_PRESSED = false;
      input.LEFT_SHIFT_JUST_PRESSED = false;
      input.LEFT_SHIFT_JUST_RELEASED = true;
    } else {
      input.LEFT_SHIFT_PRESSED = false;
      input.LEFT_SHIFT_JUST_PRESSED = false;
      input.LEFT_SHIFT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_LEFT_CONTROL:
    if (action == GLFW_PRESS) {
      input.LEFT_CONTROL_PRESSED = true;
      input.LEFT_CONTROL_JUST_PRESSED = true;
      input.LEFT_CONTROL_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_CONTROL_PRESSED = true;
      input.LEFT_CONTROL_JUST_PRESSED = false;
      input.LEFT_CONTROL_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_CONTROL_PRESSED = false;
      input.LEFT_CONTROL_JUST_PRESSED = false;
      input.LEFT_CONTROL_JUST_RELEASED = true;
    } else {
      input.LEFT_CONTROL_PRESSED = false;
      input.LEFT_CONTROL_JUST_PRESSED = false;
      input.LEFT_CONTROL_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_LEFT_ALT:
    if (action == GLFW_PRESS) {
      input.LEFT_ALT_PRESSED = true;
      input.LEFT_ALT_JUST_PRESSED = true;
      input.LEFT_ALT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_ALT_PRESSED = true;
      input.LEFT_ALT_JUST_PRESSED = false;
      input.LEFT_ALT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_ALT_PRESSED = false;
      input.LEFT_ALT_JUST_PRESSED = false;
      input.LEFT_ALT_JUST_RELEASED = true;
    } else {
      input.LEFT_ALT_PRESSED = false;
      input.LEFT_ALT_JUST_PRESSED = false;
      input.LEFT_ALT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_LEFT_SUPER:
    if (action == GLFW_PRESS) {
      input.LEFT_SUPER_PRESSED = true;
      input.LEFT_SUPER_JUST_PRESSED = true;
      input.LEFT_SUPER_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_SUPER_PRESSED = true;
      input.LEFT_SUPER_JUST_PRESSED = false;
      input.LEFT_SUPER_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_SUPER_PRESSED = false;
      input.LEFT_SUPER_JUST_PRESSED = false;
      input.LEFT_SUPER_JUST_RELEASED = true;
    } else {
      input.LEFT_SUPER_PRESSED = false;
      input.LEFT_SUPER_JUST_PRESSED = false;
      input.LEFT_SUPER_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_RIGHT_SHIFT:
    if (action == GLFW_PRESS) {
      input.RIGHT_SHIFT_PRESSED = true;
      input.RIGHT_SHIFT_JUST_PRESSED = true;
      input.RIGHT_SHIFT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_SHIFT_PRESSED = true;
      input.RIGHT_SHIFT_JUST_PRESSED = false;
      input.RIGHT_SHIFT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_SHIFT_PRESSED = false;
      input.RIGHT_SHIFT_JUST_PRESSED = false;
      input.RIGHT_SHIFT_JUST_RELEASED = true;
    } else {
      input.RIGHT_SHIFT_PRESSED = false;
      input.RIGHT_SHIFT_JUST_PRESSED = false;
      input.RIGHT_SHIFT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_RIGHT_CONTROL:
    if (action == GLFW_PRESS) {
      input.RIGHT_CONTROL_PRESSED = true;
      input.RIGHT_CONTROL_JUST_PRESSED = true;
      input.RIGHT_CONTROL_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_CONTROL_PRESSED = true;
      input.RIGHT_CONTROL_JUST_PRESSED = false;
      input.RIGHT_CONTROL_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_CONTROL_PRESSED = false;
      input.RIGHT_CONTROL_JUST_PRESSED = false;
      input.RIGHT_CONTROL_JUST_RELEASED = true;
    } else {
      input.RIGHT_CONTROL_PRESSED = false;
      input.RIGHT_CONTROL_JUST_PRESSED = false;
      input.RIGHT_CONTROL_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_RIGHT_ALT:
    if (action == GLFW_PRESS) {
      input.RIGHT_ALT_PRESSED = true;
      input.RIGHT_ALT_JUST_PRESSED = true;
      input.RIGHT_ALT_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_ALT_PRESSED = true;
      input.RIGHT_ALT_JUST_PRESSED = false;
      input.RIGHT_ALT_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_ALT_PRESSED = false;
      input.RIGHT_ALT_JUST_PRESSED = false;
      input.RIGHT_ALT_JUST_RELEASED = true;
    } else {
      input.RIGHT_ALT_PRESSED = false;
      input.RIGHT_ALT_JUST_PRESSED = false;
      input.RIGHT_ALT_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_RIGHT_SUPER:
    if (action == GLFW_PRESS) {
      input.RIGHT_SUPER_PRESSED = true;
      input.RIGHT_SUPER_JUST_PRESSED = true;
      input.RIGHT_SUPER_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_SUPER_PRESSED = true;
      input.RIGHT_SUPER_JUST_PRESSED = false;
      input.RIGHT_SUPER_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_SUPER_PRESSED = false;
      input.RIGHT_SUPER_JUST_PRESSED = false;
      input.RIGHT_SUPER_JUST_RELEASED = true;
    } else {
      input.RIGHT_SUPER_PRESSED = false;
      input.RIGHT_SUPER_JUST_PRESSED = false;
      input.RIGHT_SUPER_JUST_RELEASED = true;
    }
    break;
  case GLFW_KEY_MENU:
    if (action == GLFW_PRESS) {
      input.MENU_PRESSED = true;
      input.MENU_JUST_PRESSED = true;
      input.MENU_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.MENU_PRESSED = true;
      input.MENU_JUST_PRESSED = false;
      input.MENU_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.MENU_PRESSED = false;
      input.MENU_JUST_PRESSED = false;
      input.MENU_JUST_RELEASED = true;
    } else {
      input.MENU_PRESSED = false;
      input.MENU_JUST_PRESSED = false;
      input.MENU_JUST_RELEASED = true;
    }
    break;
  default:
    break;
  }
  if (info.scene) {
    // reverse iterate through overlay callbacks first
    bool handled = false;
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      if (layer) {
        // reverse iterate through overlay widgets
        for (auto ent_it = layer->entity_rbegin();
             ent_it != layer->entity_rend(); ent_it++) {
          Entity *entity = ent_it->get();
          if (entity) {
            auto controls_systems = entity->get_systems<IControlsSystem>();
            // reverse iterate through widget controls callbacks
            for (auto controls_it = controls_systems.rbegin();
                 controls_it != controls_systems.rend(); controls_it++) {
              if (*controls_it) {
                handled = (*controls_it)->on_key(input, entity);
                if (handled) {
                  goto end;
                }
              }
            }
          }
        }
        auto controls_systems = layer->get_systems<IControlsSystem>();
        // reverse iterate through overlay controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_key(input, layer);
            if (handled) {
              goto end;
            }
          }
        }
      }
    }
    // if event is not handled by overlays, callback to the scene
    // reverse iterate through scene entities
    for (auto entity_it = info.scene->entity_rbegin();
         entity_it != info.scene->entity_rend(); entity_it++) {
      Entity *entity = entity_it->get();
      if (entity) {
        auto controls_systems = entity->get_systems<IControlsSystem>();
        // reverse iterate through entity controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_key(input, entity);
            if (handled) {
              goto end;
            }
          }
        }
      }
    }
    auto controls_systems = info.scene->get_systems<IControlsSystem>();
    // reverse iterate through scene controls callbacks
    for (auto controls_it = controls_systems.rbegin();
         controls_it != controls_systems.rend(); controls_it++) {
      if (*controls_it) {
        handled = (*controls_it)->on_key(input, info.scene);
        if (handled) {
          goto end;
        }
      }
    }
  }
// reset just released to false after callbacks
end:
  input.SPACE_JUST_RELEASED = false;
  input.APOSTROPHE_JUST_RELEASED = false;
  input.COMMA_JUST_RELEASED = false;
  input.MINUS_JUST_RELEASED = false;
  input.PERIOD_JUST_RELEASED = false;
  input.FORWARD_SLASH_JUST_RELEASED = false;
  input.ZERO_JUST_RELEASED = false;
  input.ONE_JUST_RELEASED = false;
  input.TWO_JUST_RELEASED = false;
  input.THREE_JUST_RELEASED = false;
  input.FOUR_JUST_RELEASED = false;
  input.FIVE_JUST_RELEASED = false;
  input.SIX_JUST_RELEASED = false;
  input.SEVEN_JUST_RELEASED = false;
  input.EIGHT_JUST_RELEASED = false;
  input.NINE_JUST_RELEASED = false;
  input.SEMICOLON_JUST_RELEASED = false;
  input.EQUAL_JUST_RELEASED = false;
  input.A_JUST_RELEASED = false;
  input.B_JUST_RELEASED = false;
  input.C_JUST_RELEASED = false;
  input.D_JUST_RELEASED = false;
  input.E_JUST_RELEASED = false;
  input.F_JUST_RELEASED = false;
  input.G_JUST_RELEASED = false;
  input.H_JUST_RELEASED = false;
  input.I_JUST_RELEASED = false;
  input.J_JUST_RELEASED = false;
  input.K_JUST_RELEASED = false;
  input.L_JUST_RELEASED = false;
  input.M_JUST_RELEASED = false;
  input.N_JUST_RELEASED = false;
  input.O_JUST_RELEASED = false;
  input.P_JUST_RELEASED = false;
  input.Q_JUST_RELEASED = false;
  input.R_JUST_RELEASED = false;
  input.S_JUST_RELEASED = false;
  input.T_JUST_RELEASED = false;
  input.U_JUST_RELEASED = false;
  input.V_JUST_RELEASED = false;
  input.W_JUST_RELEASED = false;
  input.X_JUST_RELEASED = false;
  input.Y_JUST_RELEASED = false;
  input.Z_JUST_RELEASED = false;
  input.LEFT_BRACKET_JUST_RELEASED = false;
  input.RIGHT_BRACKET_JUST_RELEASED = false;
  input.GRAVE_ACCENT_JUST_RELEASED = false;
  input.ESCAPE_JUST_RELEASED = false;
  input.ENTER_JUST_RELEASED = false;
  input.TAB_JUST_RELEASED = false;
  input.BACKSPACE_JUST_RELEASED = false;
  input.INSERT_JUST_RELEASED = false;
  input.DELETE_JUST_RELEASED = false;
  input.RIGHT_JUST_RELEASED = false;
  input.LEFT_JUST_RELEASED = false;
  input.UP_JUST_RELEASED = false;
  input.DOWN_JUST_RELEASED = false;
  input.PAGE_UP_JUST_RELEASED = false;
  input.PAGE_DOWN_JUST_RELEASED = false;
  input.HOME_JUST_RELEASED = false;
  input.END_JUST_RELEASED = false;
  input.CAPS_LOCK_JUST_RELEASED = false;
  input.SCROLL_LOCK_JUST_RELEASED = false;
  input.NUM_LOCK_JUST_RELEASED = false;
  input.PRINT_SCREEN_JUST_RELEASED = false;
  input.PAUSE_JUST_RELEASED = false;
  input.F1_JUST_RELEASED = false;
  input.F2_JUST_RELEASED = false;
  input.F3_JUST_RELEASED = false;
  input.F4_JUST_RELEASED = false;
  input.F5_JUST_RELEASED = false;
  input.F6_JUST_RELEASED = false;
  input.F7_JUST_RELEASED = false;
  input.F8_JUST_RELEASED = false;
  input.F9_JUST_RELEASED = false;
  input.F10_JUST_RELEASED = false;
  input.F11_JUST_RELEASED = false;
  input.F12_JUST_RELEASED = false;
  input.KEY_PAD_0_JUST_RELEASED = false;
  input.KEY_PAD_1_JUST_RELEASED = false;
  input.KEY_PAD_2_JUST_RELEASED = false;
  input.KEY_PAD_3_JUST_RELEASED = false;
  input.KEY_PAD_4_JUST_RELEASED = false;
  input.KEY_PAD_5_JUST_RELEASED = false;
  input.KEY_PAD_6_JUST_RELEASED = false;
  input.KEY_PAD_7_JUST_RELEASED = false;
  input.KEY_PAD_8_JUST_RELEASED = false;
  input.KEY_PAD_9_JUST_RELEASED = false;
  input.KEY_PAD_DECIMAL_JUST_RELEASED = false;
  input.KEY_PAD_DIVIDE_JUST_RELEASED = false;
  input.KEY_PAD_MULTIPLY_JUST_RELEASED = false;
  input.KEY_PAD_SUBTRACT_JUST_RELEASED = false;
  input.KEY_PAD_ADD_JUST_RELEASED = false;
  input.KEY_PAD_ENTER_JUST_RELEASED = false;
  input.KEY_PAD_EQUAL_JUST_RELEASED = false;
  input.LEFT_SHIFT_JUST_RELEASED = false;
  input.LEFT_CONTROL_JUST_RELEASED = false;
  input.LEFT_ALT_JUST_RELEASED = false;
  input.LEFT_SUPER_JUST_RELEASED = false;
  input.RIGHT_SHIFT_JUST_RELEASED = false;
  input.RIGHT_CONTROL_JUST_RELEASED = false;
  input.RIGHT_ALT_JUST_RELEASED = false;
  input.RIGHT_SUPER_JUST_RELEASED = false;
  input.MENU_JUST_RELEASED = false;
}

void GLRenderer::glfw_onMouseButton(GLFWwindow *window, int button, int action,
                                    int mods) {
  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT:
    if (action == GLFW_PRESS) {
      input.LEFT_MOUSE_PRESSED = true;
      input.LEFT_MOUSE_JUST_PRESSED = true;
      input.LEFT_MOUSE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.LEFT_MOUSE_PRESSED = true;
      input.LEFT_MOUSE_JUST_PRESSED = false;
      input.LEFT_MOUSE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.LEFT_MOUSE_PRESSED = false;
      input.LEFT_MOUSE_JUST_PRESSED = false;
      input.LEFT_MOUSE_JUST_RELEASED = true;
    } else {
      input.LEFT_MOUSE_PRESSED = false;
      input.LEFT_MOUSE_JUST_PRESSED = false;
      input.LEFT_MOUSE_JUST_RELEASED = true;
    }
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    if (action == GLFW_PRESS) {
      input.RIGHT_MOUSE_PRESSED = true;
      input.RIGHT_MOUSE_JUST_PRESSED = true;
      input.RIGHT_MOUSE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.RIGHT_MOUSE_PRESSED = true;
      input.RIGHT_MOUSE_JUST_PRESSED = false;
      input.RIGHT_MOUSE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.RIGHT_MOUSE_PRESSED = false;
      input.RIGHT_MOUSE_JUST_PRESSED = false;
      input.RIGHT_MOUSE_JUST_RELEASED = true;
    } else {
      input.RIGHT_MOUSE_PRESSED = false;
      input.RIGHT_MOUSE_JUST_PRESSED = false;
      input.RIGHT_MOUSE_JUST_RELEASED = true;
    }
    break;
  case GLFW_MOUSE_BUTTON_MIDDLE:
    if (action == GLFW_PRESS) {
      input.MIDDLE_MOUSE_PRESSED = true;
      input.MIDDLE_MOUSE_JUST_PRESSED = true;
      input.MIDDLE_MOUSE_JUST_RELEASED = false;
    } else if (action == GLFW_REPEAT) {
      input.MIDDLE_MOUSE_PRESSED = true;
      input.MIDDLE_MOUSE_JUST_PRESSED = false;
      input.MIDDLE_MOUSE_JUST_RELEASED = false;
    } else if (action == GLFW_RELEASE) {
      input.MIDDLE_MOUSE_PRESSED = false;
      input.MIDDLE_MOUSE_JUST_PRESSED = false;
      input.MIDDLE_MOUSE_JUST_RELEASED = true;
    } else {
      input.MIDDLE_MOUSE_PRESSED = false;
      input.MIDDLE_MOUSE_JUST_PRESSED = false;
      input.MIDDLE_MOUSE_JUST_RELEASED = true;
    }
    break;
  }
  if (info.scene) {
    // reverse iterate through layer callbacks first
    bool handled = false;
    for (auto layer_it = info.scene->layer_rbegin();
         layer_it != info.scene->layer_rend(); layer_it++) {
      Layer *layer = layer_it->get();
      if (layer) {
        // reverse iterate through overlay entities
        for (auto entity_it = layer->entity_rbegin();
             entity_it != layer->entity_rend(); entity_it++) {
          Entity *entity = entity_it->get();
          if (entity) {
            auto controls_systems = entity->get_systems<IControlsSystem>();
            // reverse iterate through entity controls callbacks
            for (auto controls_it = controls_systems.rbegin();
                 controls_it != controls_systems.rend(); controls_it++) {
              handled = (*controls_it)->on_mouse_button(input, entity);
              if (handled) {
                goto end;
              }
            }
          }
        }
        auto controls_systems = layer->get_systems<IControlsSystem>();
        // reverse iterate through layer controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_mouse_button(input, layer);
            if (handled) {
              goto end;
            }
          }
        }
      }
    }
    // if event is not handled by layers, unfocus UIElements and callback to the
    // scene reverse iterate through scene entities
    UIElement::focus(nullptr);
    for (auto entity_it = info.scene->entity_rbegin();
         entity_it != info.scene->entity_rend(); entity_it++) {
      Entity *entity = entity_it->get();
      if (entity) {
        auto controls_systems = entity->get_systems<IControlsSystem>();
        // reverse iterate through entity controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_mouse_button(input, entity);
            if (handled) {
              goto end;
            }
          }
        }
      }
    }
    auto controls_systems = info.scene->get_systems<IControlsSystem>();
    // reverse iterate through scene controls callbacks
    for (auto controls_it = controls_systems.rbegin();
         controls_it != controls_systems.rend(); controls_it++) {
      if (*controls_it) {
        handled = (*controls_it)->on_mouse_button(input, info.scene);
        if (handled) {
          goto end;
        }
      }
    }
  }
end:
  input.LEFT_MOUSE_JUST_RELEASED = false;
  input.MIDDLE_MOUSE_JUST_RELEASED = false;
  input.RIGHT_MOUSE_JUST_RELEASED = false;
  input.LEFT_MOUSE_JUST_PRESSED = false;
  input.MIDDLE_MOUSE_JUST_PRESSED = false;
  input.RIGHT_MOUSE_JUST_PRESSED = false;
}

void GLRenderer::glfw_onMouseMove(GLFWwindow *window, double x, double y) {
  if (!info.cursor) {
    info.cursor = true;
    input.mouse_pos = glm::ivec2(x, y);
  }
  glm::ivec2 old_pos = input.mouse_pos;
  input.mouse_pos = glm::ivec2(x, y);
  input.mouse_vel = glm::ivec2(x, y) - old_pos;
  if (info.scene) {
    // reverse iterate through layer callbacks first
    bool handled = false;
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      if (layer) {
        // reverse iterate through layer entities
        for (auto ent_it = layer->entity_rbegin();
             ent_it != layer->entity_rend(); ent_it++) {
          Entity *entity = ent_it->get();
          if (entity) {
            auto controls_systems = entity->get_systems<IControlsSystem>();
            // reverse iterate through entity controls callbacks
            for (auto controls_it = controls_systems.rbegin();
                 controls_it != controls_systems.rend(); controls_it++) {
              if (*controls_it) {
                handled = (*controls_it)->on_mouse_move(input, entity);
                if (handled) {
                  // mouse velocity is always zero outside of mouse move
                  // callbacks
                  input.mouse_vel = glm::ivec2(0, 0);
                  return;
                }
              }
            }
          }
        }
        auto controls_systems = layer->get_systems<IControlsSystem>();
        // reverse iterate through layer controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_mouse_move(input, layer);
            if (handled) {
              // mouse velocity is always zero outside of mouse move callbacks
              input.mouse_vel = glm::ivec2(0, 0);
              return;
            }
          }
        }
      }
    }
    // if event is not handled by overlays, callback to the scene
    // reverse iterate through scene entities
    for (auto entity_it = info.scene->entity_rbegin();
         entity_it != info.scene->entity_rend(); entity_it++) {
      Entity *entity = entity_it->get();
      if (entity) {
        auto controls_systems = entity->get_systems<IControlsSystem>();
        // reverse iterate through entity controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_mouse_move(input, entity);
            if (handled) {
              // mouse velocity is always zero outside of mouse move callbacks
              input.mouse_vel = glm::ivec2(0, 0);
              return;
            }
          }
        }
      }
    }
    auto controls_systems = info.scene->get_systems<IControlsSystem>();
    // reverse iterate through scene controls callbacks
    for (auto controls_it = controls_systems.rbegin();
         controls_it != controls_systems.rend(); controls_it++) {
      if (*controls_it) {
        handled = (*controls_it)->on_mouse_move(input, info.scene);
        if (handled) {
          // mouse velocity is always zero outside of mouse move callbacks
          input.mouse_vel = glm::ivec2(0, 0);
          return;
        }
      }
    }
  }
  // mouse velocity is always zero outside of mouse move callbacks
  input.mouse_vel = glm::ivec2(0, 0);
}

void GLRenderer::glfw_onMouseWheel(GLFWwindow *window, double xoffset,
                                   double yoffset) {
  if (yoffset > 0) {
    input.mouse_scroll = 1;
  }
  if (yoffset < 0) {
    input.mouse_scroll = -1;
  }
  if (info.scene) {
    // reverse iterate through layer callbacks first
    bool handled = false;
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      if (layer) {
        // reverse iterate through layer entities
        for (auto ent_it = layer->entity_rbegin();
             ent_it != layer->entity_rend(); ent_it++) {
          Entity *entity = ent_it->get();
          if (entity) {
            auto controls_systems = entity->get_systems<IControlsSystem>();
            // reverse iterate through widget controls callbacks
            for (auto controls_it = controls_systems.rbegin();
                 controls_it != controls_systems.rend(); controls_it++) {
              if (*controls_it) {
                handled = (*controls_it)->on_mouse_wheel(input, entity);
                if (handled) {
                  // mouse scroll is always 0 outside of mouse scroll callbacks
                  input.mouse_scroll = 0;
                  return;
                }
              }
            }
          }
        }
        auto controls_systems = layer->get_systems<IControlsSystem>();
        // reverse iterate through overlay controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_mouse_wheel(input, layer);
            if (handled) {
              // mouse scroll is always 0 outside of mouse scroll callbacks
              input.mouse_scroll = 0;
              return;
            }
          }
        }
      }
    }
    // if event is not handled by overlays, callback to the scene
    // reverse iterate through scene entities
    for (auto entity_it = info.scene->entity_rbegin();
         entity_it != info.scene->entity_rend(); entity_it++) {
      Entity *entity = entity_it->get();
      if (entity) {
        auto controls_systems = entity->get_systems<IControlsSystem>();
        // reverse iterate through entity controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled = (*controls_it)->on_mouse_wheel(input, entity);
            if (handled) {
              // mouse scroll is always 0 outside of mouse scroll callbacks
              input.mouse_scroll = 0;
              return;
            }
          }
        }
      }
    }
    auto controls_systems = info.scene->get_systems<IControlsSystem>();
    // reverse iterate through scene controls callbacks
    for (auto controls_it = controls_systems.rbegin();
         controls_it != controls_systems.rend(); controls_it++) {
      if (*controls_it) {
        handled = (*controls_it)->on_mouse_wheel(input, info.scene);
        if (handled) {
          // mouse scroll is always 0 outside of mouse scroll callbacks
          input.mouse_scroll = 0;
          return;
        }
      }
    }
  }
  // mouse scroll is always 0 outside of mouse scroll callbacks
  input.mouse_scroll = 0;
}

void GLRenderer::glfw_onChar(GLFWwindow *window, unsigned int code_point) {
  if (info.scene) {
    // reverse iterate through layer callbacks first
    bool handled = false;
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      if (layer) {
        // reverse iterate through layer entities
        for (auto ent_it = layer->entity_rbegin();
             ent_it != layer->entity_rend(); ent_it++) {
          Entity *entity = ent_it->get();
          if (entity) {
            auto controls_systems = entity->get_systems<IControlsSystem>();
            // reverse iterate through widget controls callbacks
            for (auto controls_it = controls_systems.rbegin();
                 controls_it != controls_systems.rend(); controls_it++) {
              if (*controls_it) {
                handled = (*controls_it)
                              ->on_char(static_cast<char>(code_point), entity);
                if (handled) {
                  return;
                }
              }
            }
          }
        }
        auto controls_systems = layer->get_systems<IControlsSystem>();
        // reverse iterate through layer controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled =
                (*controls_it)->on_char(static_cast<char>(code_point), layer);
            if (handled) {
              return;
            }
          }
        }
      }
    }
    // if event is not handled by layers, callback to the scene
    // reverse iterate through scene entities
    for (auto entity_it = info.scene->entity_rbegin();
         entity_it != info.scene->entity_rend(); entity_it++) {
      Entity *entity = entity_it->get();
      if (entity) {
        auto controls_systems = entity->get_systems<IControlsSystem>();
        // reverse iterate through entity controls callbacks
        for (auto controls_it = controls_systems.rbegin();
             controls_it != controls_systems.rend(); controls_it++) {
          if (*controls_it) {
            handled =
                (*controls_it)->on_char(static_cast<char>(code_point), entity);
            if (handled) {
              return;
            }
          }
        }
      }
    }
    auto controls_systems = info.scene->get_systems<IControlsSystem>();
    // reverse iterate through scene controls callbacks
    for (auto controls_it = controls_systems.rbegin();
         controls_it != controls_systems.rend(); controls_it++) {
      if (*controls_it) {
        handled =
            (*controls_it)->on_char(static_cast<char>(code_point), info.scene);
        if (handled) {
          return;
        }
      }
    }
  }
}

// Initialize static variable for the window
GLFWwindow *GLRenderer::window = nullptr;

} // namespace mare
