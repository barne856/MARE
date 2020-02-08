#include "mare/Renderer.hpp"
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
Camera::~Camera()
{
    if (m_controls)
    {
        delete m_controls;
        m_controls = nullptr;
    }
}
glm::mat4 Camera::projection() const { return m_projection; }
glm::mat4 Camera::view() const { return m_view; }
void Camera::set_controls(ControlsConfig config)
{
    if (m_controls)
    {
        delete m_controls;
        m_controls = nullptr;
    }

    switch (config)
    {
    case ControlsConfig::ORBITCONTROLS:
        m_controls = new OrbitControls();
        break;
    default:
        m_controls = nullptr;
        break;
    }
}
void Camera::interpret_input()
{
    if (m_controls)
    {
        m_controls->interpret_input(this, Renderer::API->get_input());
    }
}
void Camera::set_view(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
    m_position = eye;
    m_direction = center - eye;
    m_direction = glm::normalize(m_direction);
    m_up = glm::normalize(up);
    recalculate_view();
}
void Camera::set_position(const glm::vec3 &position)
{
    m_position = position;
    recalculate_view();
}
void Camera::set_direction(const glm::vec3 &direction)
{
    m_direction = direction;
    m_direction = glm::normalize(m_direction);
    recalculate_view();
}
void Camera::set_up(const glm::vec3 &up)
{
    m_up = up;
    m_up = glm::normalize(m_up);
    recalculate_view();
}
void Camera::set_fov(float fovy)
{
    m_fovy = fovy;
    recalculate_projection();
}
void Camera::set_scale(float scale)
{
    m_scale = scale;
    recalculate_projection();
}
void Camera::set_aspect(float aspect)
{
    m_aspect = aspect;
    recalculate_projection();
}
void Camera::set_near_clip_plan(float near)
{
    m_near = near;
    recalculate_projection();
}
void Camera::set_far_clip_plan(float far)
{
    m_far = far;
    recalculate_projection();
}
void Camera::set_forward_velocity(float velocity)
{
    m_linear_velocity = m_direction * velocity;
}
void Camera::set_linear_velocity(const glm::vec3 &linear_velocity)
{
    m_linear_velocity = linear_velocity;
}
void Camera::set_angular_velocity(float angular_velocity)
{
    m_angular_velocity = angular_velocity;
}
void Camera::set_distance_to_center(float distance)
{
    m_distance_to_center = distance;
}
glm::vec3 Camera::get_position() const { return m_position; }
glm::vec3 Camera::get_direction() const { return m_direction; }
glm::vec3 Camera::get_up() const { return m_up; }
float Camera::get_distance_to_center() const { return m_distance_to_center; }
void Camera::render(double dt)
{
    set_position(m_position + m_linear_velocity * float(dt));
    set_direction(glm::vec3(glm::rotate(glm::mat4(1.0f), m_angular_velocity * float(dt), m_up) * glm::vec4(m_direction, 1.0f)));
}
void Camera::recalculate_projection()
{
    m_aspect = Renderer::API->get_info().window_aspect;
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
glm::vec3 Camera::screen_to_world(glm::ivec2 screen_coords)
{
    // does not calculate z value yet
    glm::mat4 inversed_camera = glm::inverse(m_projection * m_view);
    float x = 2.0f * (float)screen_coords.x / (float)(Renderer::API->get_info().window_width) - 1.0f;
    float y = -2.0f * (float)screen_coords.y / (float)(Renderer::API->get_info().window_height) + 1.0f;
    glm::vec4 screen_vector = glm::vec4(x, y, 0.0f, 1.0f);
    glm::vec4 world_vector = inversed_camera * screen_vector;
    world_vector /= world_vector.w;
    return glm::vec3(world_vector);
}
} // namespace mare