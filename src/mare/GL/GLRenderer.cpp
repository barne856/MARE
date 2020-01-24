#include "mare/GL/GLRenderer.hpp"
#include "mare/GL/GLBuffer.hpp"
#include "mare/GL/GLShader.hpp"
#include "mare/GL/GLRenderState.hpp"
#include "mare/Application.hpp"
#include "mare/SimpleMesh.hpp"

// Standard Library
#include <iostream>

namespace mare
{

namespace GL
{

GLenum GLDrawMethod(DrawMethod draw_method)
{
    switch (draw_method)
    {
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
// Meshes
// normal rendering
template <typename T>
void render_mesh(SimpleMesh<T> *mesh, Material *material)
{
    mesh->bind();
    material->bind();
    if (Application::get_camera())
    {
        material->upload_mat4("projection", Application::get_camera()->projection());
        material->upload_mat4("view", Application::get_camera()->view());
    }
    material->upload_mat4("model", mesh->get_model());
    if (mesh->get_state()->is_indexed())
    {
        glDrawElements(GLDrawMethod(mesh->get_draw_method()), GLsizei(mesh->get_state()->render_count()), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GLDrawMethod(mesh->get_draw_method()), 0, GLsizei(mesh->get_state()->render_count()));
    }
}
// composite rendering
template <typename T>
void render_mesh(SimpleMesh<T> *mesh, Material *material, glm::mat4 parent_model)
{
    mesh->bind();
    material->bind();
    if (Application::get_camera())
    {
        material->upload_mat4("projection", Application::get_camera()->projection());
        material->upload_mat4("view", Application::get_camera()->view());
    }
    material->upload_mat4("model", parent_model * mesh->get_model());
    if (mesh->get_state()->is_indexed())
    {
        glDrawElements(GLDrawMethod(mesh->get_draw_method()), GLsizei(mesh->get_state()->render_count()), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GLDrawMethod(mesh->get_draw_method()), 0, GLsizei(mesh->get_state()->render_count()));
    }
}
// instanced rendering
template <typename T>
void render_mesh(SimpleMesh<T> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    mesh->bind();
    material->bind();
    if (Application::get_camera())
    {
        material->upload_mat4("projection", Application::get_camera()->projection());
        material->upload_mat4("view", Application::get_camera()->view());
    }
    material->upload_mat4("model", parent_model * mesh->get_model());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, models->name());
    if (mesh->get_state()->is_indexed())
    {
        glDrawElementsInstanced(GLDrawMethod(mesh->get_draw_method()), mesh->get_state()->render_count(), GL_UNSIGNED_INT, nullptr, instance_count);
    }
    else
    {
        glDrawArraysInstanced(GLDrawMethod(mesh->get_draw_method()), 0, mesh->get_state()->render_count(), instance_count);
    }
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

} // namespace GL

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
        double delta_time = time - info.current_time;
        if (m_layer_stack)
        {
            bool render_result = true;
            for (auto &layer : *m_layer_stack)
            {
                render_result = layer->render(time, delta_time);
                if (!render_result)
                {
                    m_layer_stack = nullptr;
                    break;
                }
            }
        }
        m_app_pointer->render(time, delta_time);
        if (m_camera_pointer)
        {
            m_camera_pointer->render(delta_time);
        }
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

// Buffers
Buffer<float> *GLRenderer::GenFloatBuffer(unsigned int count)
{
    return new GLBuffer<float>[count];
}
Buffer<int> *GLRenderer::GenIntBuffer(unsigned int count)
{
    return new GLBuffer<int>[count];
}
Buffer<unsigned int> *GLRenderer::GenIndexBuffer(unsigned int count)
{
    return new GLBuffer<unsigned int>[count];
}
Buffer<bool> *GLRenderer::GenBoolBuffer(unsigned int count)
{
    return new GLBuffer<bool>[count];
}
Buffer<glm::mat4> *GLRenderer::GenMat4Buffer(unsigned int count)
{
    return new GLBuffer<glm::mat4>[count];
}
Buffer<glm::mat3> *GLRenderer::GenMat3Buffer(unsigned int count)
{
    return new GLBuffer<glm::mat3>[count];
}
Buffer<glm::mat2> *GLRenderer::GenMat2Buffer(unsigned int count)
{
    return new GLBuffer<glm::mat2>[count];
}
Buffer<glm::vec2> *GLRenderer::GenVec2Buffer(unsigned int count)
{
    return new GLBuffer<glm::vec2>[count];
}
Buffer<glm::vec3> *GLRenderer::GenVec3Buffer(unsigned int count)
{
    return new GLBuffer<glm::vec3>[count];
}
Buffer<glm::vec4> *GLRenderer::GenVec4Buffer(unsigned int count)
{
    return new GLBuffer<glm::vec4>[count];
}

// Render States
RenderState<float> *GLRenderer::GenFloatRenderState()
{
    return new GLRenderState<float>();
}
RenderState<glm::vec2> *GLRenderer::GenVec2RenderState()
{
    return new GLRenderState<glm::vec2>();
}
RenderState<glm::vec3> *GLRenderer::GenVec3RenderState()
{
    return new GLRenderState<glm::vec3>();
}
RenderState<glm::vec4> *GLRenderer::GenVec4RenderState()
{
    return new GLRenderState<glm::vec4>();
}

// Render Meshes
void GLRenderer::render_float_mesh(SimpleMesh<float> *mesh, Material *material)
{
    GL::render_mesh(mesh, material);
}
void GLRenderer::render_float_mesh(SimpleMesh<float> *mesh, Material *material, glm::mat4 parent_model)
{
    GL::render_mesh(mesh, material, parent_model);
}
void GLRenderer::render_float_mesh(SimpleMesh<float> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    GL::render_mesh(mesh, material, parent_model, instance_count, models);
}
void GLRenderer::render_vec2_mesh(SimpleMesh<glm::vec2> *mesh, Material *material)
{
    GL::render_mesh(mesh, material);
}
void GLRenderer::render_vec2_mesh(SimpleMesh<glm::vec2> *mesh, Material *material, glm::mat4 parent_model)
{
    GL::render_mesh(mesh, material, parent_model);
}
void GLRenderer::render_vec2_mesh(SimpleMesh<glm::vec2> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    GL::render_mesh(mesh, material, parent_model, instance_count, models);
}
void GLRenderer::render_vec3_mesh(SimpleMesh<glm::vec3> *mesh, Material *material)
{
    GL::render_mesh(mesh, material);
}
void GLRenderer::render_vec3_mesh(SimpleMesh<glm::vec3> *mesh, Material *material, glm::mat4 parent_model)
{
    GL::render_mesh(mesh, material, parent_model);
}
void GLRenderer::render_vec3_mesh(SimpleMesh<glm::vec3> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    GL::render_mesh(mesh, material, parent_model, instance_count, models);
}
void GLRenderer::render_vec4_mesh(SimpleMesh<glm::vec4> *mesh, Material *material)
{
    GL::render_mesh(mesh, material);
}
void GLRenderer::render_vec4_mesh(SimpleMesh<glm::vec4> *mesh, Material *material, glm::mat4 parent_model)
{
    GL::render_mesh(mesh, material, parent_model);
}
void GLRenderer::render_vec4_mesh(SimpleMesh<glm::vec4> *mesh, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    GL::render_mesh(mesh, material, parent_model, instance_count, models);
}

Shader *GLRenderer::GenShader(const char *directory)
{
    return new GLShader(directory);
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
}

void GLRenderer::glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS)
        {
            input.W_PRESSED = true;
        }
        else
        {
            input.W_PRESSED = false;
        }
    }
    if (m_layer_stack)
    {
        bool handled = false;
        for (size_t i = m_layer_stack->size(); i--;)
        {
            handled = m_layer_stack->at(i)->on_key(input);
            if (handled)
            {
                break;
            }
        }
    }
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
    if (m_layer_stack)
    {
        bool handled = false;
        for (size_t i = m_layer_stack->size(); i--;)
        {
            handled = m_layer_stack->at(i)->on_mouse_button(input);
            if (handled)
            {
                break;
            }
        }
    }
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
    if (m_layer_stack)
    {
        bool handled = false;
        for (size_t i = m_layer_stack->size(); i--;)
        {
            handled = m_layer_stack->at(i)->on_mouse_move(input);
            if (handled)
            {
                break;
            }
        }
    }
    get_input().mouse_vel = glm::ivec2(0, 0);
}

void GLRenderer::glfw_onMouseWheel(GLFWwindow *window, double xoffset, double yoffset)
{
    if (m_layer_stack)
    {
        bool handled = false;
        for (size_t i = m_layer_stack->size(); i--;)
        {
            handled = m_layer_stack->at(i)->on_mouse_wheel(input);
            if (handled)
            {
                break;
            }
        }
    }
}

GLFWwindow *GLRenderer::window = nullptr;

} // namespace mare
