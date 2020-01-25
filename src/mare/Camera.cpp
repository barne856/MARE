#include "mare/Application.hpp"
#include "mare/Camera.hpp"

namespace mare
{
Camera::Camera(CameraType type)
    : m_controls(nullptr), m_linear_velocity(glm::vec3(0.0f)), m_angular_velocity(0.0f), m_position(glm::vec3(0.0f)), m_direction(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(glm::vec3(0.0f, 1.0f, 0.0f))
{
    m_type = type;
    if (type == CameraType::PERSPECTIVE)
    {
        m_near = 0.01f;
        m_far = 1000.0f;
    }
    recalculate_projection();
    recalculate_view();
}

void Camera::recalculate_projection()
{
    m_aspect = Application::get_info().window_aspect;
    switch (m_type)
    {
    case CameraType::PERSPECTIVE:
        m_projection = glm::perspective(glm::radians(m_fovy), m_aspect, m_near, m_far);
        break;
    case CameraType::ORTHOGRAPHIC:
        m_projection = glm::ortho(-m_scale * m_aspect, m_scale * m_aspect, -m_scale, m_scale);
        break;
    }
}
void Camera::recalculate_view()
{
    m_view = glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::interpret_input()
{
    if (m_controls)
    {
        m_controls->interpret_input(this, Application::get_input());
    }
}

glm::vec3 Camera::screen_to_world(glm::ivec2 screen_coords)
{
    // does not calculate z value yet
    glm::mat4 inversed_camera = glm::inverse(m_projection * m_view);
    float x = 2.0f * (float)screen_coords.x / (float)(Application::get_info().window_width) - 1.0f;
    float y = -2.0f * (float)screen_coords.y / (float)(Application::get_info().window_height) + 1.0f;
    glm::vec4 screen_vector = glm::vec4(x, y, 0.0f, 1.0f);
    glm::vec4 world_vector = inversed_camera * screen_vector;
    world_vector /= world_vector.w;
    return glm::vec3(world_vector);
}

void Camera::render(double dt)
{
    set_position(m_position + m_linear_velocity * float(dt));
    set_direction(glm::vec3(glm::rotate(glm::mat4(1.0f), m_angular_velocity * float(dt), m_up) * glm::vec4(m_direction, 1.0f)));
}
} // namespace mare