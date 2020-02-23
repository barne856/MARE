#include "mare/Renderer.hpp"
#include "mare/Camera.hpp"

namespace mare
{
Camera::Camera(ProjectionType type)
    : type_(type), controls_(nullptr), linear_velocity_(glm::vec3(0.0f)), angular_velocity_(0.0f), position_(glm::vec3(0.0f)), direction_(glm::vec3(0.0f, 0.0f, -1.0f)), up_(glm::vec3(0.0f, 1.0f, 0.0f))
{
    if (type == ProjectionType::PERSPECTIVE)
    {
        near_ = 0.1f;
        far_ = 10.0f;
    }
    recalculate_projection();
    recalculate_view();
}
Camera::~Camera() {}
glm::mat4 Camera::projection() const { return projection_; }
glm::mat4 Camera::view() const { return view_; }
void Camera::set_controls(Scoped<Controls> controls)
{
    controls_ = std::move(controls);
}
void Camera::interpret_input()
{
    if (controls_)
    {
        controls_->interpret_input(this, Renderer::API->get_input());
    }
}
void Camera::set_view(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
    position_ = eye;
    direction_ = glm::normalize(center - eye);
    up_ = glm::normalize(up);
    recalculate_view();
}
void Camera::set_position(const glm::vec3 &position)
{
    position_ = position;
    recalculate_view();
}
void Camera::set_direction(const glm::vec3 &direction)
{
    direction_ = glm::normalize(direction);
    recalculate_view();
}
void Camera::set_up(const glm::vec3 &up)
{
    up_ = glm::normalize(up);
    recalculate_view();
}
void Camera::set_fov(float fovy)
{
    fovy_ = fovy;
    recalculate_projection();
}
void Camera::set_scale(float scale)
{
    scale_ = scale;
    recalculate_projection();
}
void Camera::set_aspect(float aspect)
{
    aspect_ = aspect;
    recalculate_projection();
}
void Camera::set_near_clip_plan(float near)
{
    near_ = near;
    recalculate_projection();
}
void Camera::set_far_clip_plan(float far)
{
    far_ = far;
    recalculate_projection();
}
void Camera::set_forward_velocity(float velocity)
{
    linear_velocity_ = direction_ * velocity;
}
void Camera::set_linear_velocity(const glm::vec3 &linear_velocity)
{
    linear_velocity_ = linear_velocity;
}
void Camera::set_angular_velocity(float angular_velocity)
{
    angular_velocity_ = angular_velocity;
}
void Camera::set_distance_to_center(float distance)
{
    distance_to_center_ = distance;
}
glm::vec3 Camera::get_position() const { return position_; }
glm::vec3 Camera::get_direction() const { return direction_; }
glm::vec3 Camera::get_up() const { return up_; }
float Camera::get_distance_to_center() const { return distance_to_center_; }
void Camera::render(double dt)
{
    set_position(position_ + linear_velocity_ * float(dt));
    set_direction(glm::vec3(glm::rotate(glm::mat4(1.0f), angular_velocity_ * float(dt), up_) * glm::vec4(direction_, 1.0f)));
}
void Camera::recalculate_projection()
{
    aspect_ = Renderer::API->get_info().window_aspect;
    switch (type_)
    {
    case ProjectionType::PERSPECTIVE:
        projection_ = glm::perspective(glm::radians(fovy_), aspect_, near_, far_);
        break;
    case ProjectionType::ORTHOGRAPHIC:
        projection_ = glm::ortho(-scale_ * aspect_, scale_ * aspect_, -scale_, scale_);
        break;
    }
}
void Camera::recalculate_view()
{
    view_ = glm::lookAt(position_, position_ + direction_, up_);
}
} // namespace mare