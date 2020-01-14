#ifndef CAMERA
#define CAMERA

// MARE
#include "mare/Controls.hpp"

#include "ext/matrix_transform.hpp"  // glm::translate, glm::rotate, glm::scale
#include "ext/matrix_clip_space.hpp" // glm::perspective

namespace mare
{

enum class CameraType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera
{
public:
    Camera(CameraType type);
    ~Camera()
    {
        if (m_controls)
        {
            delete m_controls;
        }
    }

    glm::mat4 inline projection() const { return m_projection; }
    glm::mat4 inline view() const { return m_view; }

    void set_controls(ControlsConfig config)
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
            m_controls = new OrbitControls();
            break;
        }
    }

    void interpret_input();

    void set_view(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
    {
        m_position = eye;
        m_direction = center - eye;
        glm::normalize(m_direction);
        m_up = glm::normalize(up);
        recalculate_view();
    }
    void set_position(const glm::vec3 &position)
    {
        m_position = position;
        recalculate_view();
    }
    void set_direction(const glm::vec3 &direction)
    {
        m_direction = direction;
        glm::normalize(m_direction);
        recalculate_view();
    }
    void set_up(const glm::vec3& up)
    {
        m_up = up;
        glm::normalize(m_up);
        recalculate_view();
    }
    void set_forward_velocity(float velocity)
    {
        m_linear_velocity = m_direction * velocity;
    }
    void set_linear_velocity(const glm::vec3 &linear_velocity)
    {
        m_linear_velocity = linear_velocity;
    }
    void set_angular_velocity(float angular_velocity)
    {
        m_angular_velocity = angular_velocity;
    }

    // Pespective FOV in y direction
    void set_fov(float fovy) { m_fovy = fovy; }
    // Orthographic scale in y direction
    void set_scale(float scale) { m_scale = scale; }

    void set_distance_to_center(float distance)
    {
        m_distance_to_center = distance;
    }
    void set_aspect(float aspect) { m_aspect = aspect; }
    void set_near_clip_plan(float near) { m_near = near; }
    void set_far_clip_plan(float far) { m_far = far; }

    glm::vec3 get_position() const { return m_position; }
    glm::vec3 get_direction() const { return m_direction; }
    glm::vec3 get_up() const { return m_up; }
    float get_distance_to_center() const { return m_distance_to_center; }

    void render(double dt);
    void recalculate_projection();
    void recalculate_view();

private:
    Controls *m_controls;

    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::vec3 m_linear_velocity;
    float m_angular_velocity;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;

    float m_scale = 1.0f;
    float m_aspect = 1.0f;
    float m_near = 0.01f;
    float m_far = 1000.0f;
    float m_fovy = 45.0f;
    float m_distance_to_center = 1.0f;

    CameraType m_type;
};
} // namespace mare

#endif