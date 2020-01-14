#include "mare/Application.hpp"
#include "mare/Camera.hpp"

namespace mare
{
Camera::Camera(CameraType type)
    : m_controls(nullptr), m_linear_velocity(glm::vec3(0.0f)), m_angular_velocity(0.0f), m_position(glm::vec3(0.0f)), m_direction(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(glm::vec3(0.0f, 1.0f, 0.0f))
{
    m_type = type;
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

void Camera::render(double dt)
{
    set_position(m_position + m_linear_velocity*float(dt));
    set_direction(glm::vec3(glm::rotate(glm::mat4(1.0f), m_angular_velocity*float(dt), m_up) * glm::vec4(m_direction, 1.0f)));
}
} // namespace mare