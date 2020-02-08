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
    ~Camera();

    // Get projection and view matrices from the camera
    glm::mat4 projection() const;
    glm::mat4 view() const;

    // Set the controls for the camera
    void set_controls(ControlsConfig config);

    // Interpret input from renderer using camera controls
    void interpret_input();

    // Set camera properties
    void set_view(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
    void set_position(const glm::vec3 &position);
    void set_direction(const glm::vec3 &direction);
    void set_up(const glm::vec3 &up);
    void set_fov(float fovy);    // Pespective FOV in y direction
    void set_scale(float scale); // Orthographic scale in y direction
    void set_aspect(float aspect);
    void set_near_clip_plan(float near);
    void set_far_clip_plan(float far);
    void set_forward_velocity(float velocity);
    void set_linear_velocity(const glm::vec3 &linear_velocity);
    void set_angular_velocity(float angular_velocity);
    void set_distance_to_center(float distance);

    // Get camera properties
    glm::vec3 get_position() const;
    glm::vec3 get_direction() const;
    glm::vec3 get_up() const;
    float get_distance_to_center() const;

    // Update the camera
    void render(double dt);        //Physics
    void recalculate_projection(); // projection
    void recalculate_view();       // view

    // Raycast screen coordinates to a 3D point in world space
    glm::vec3 screen_to_world(glm::ivec2 screen_coords);

private:
    // Camera Controls
    Controls *m_controls;

    // Camera Properties
    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::vec3 m_linear_velocity;
    float m_angular_velocity;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    float m_scale = 1.0f;
    float m_aspect = 1.0f;
    float m_near = -1.0f;
    float m_far = 1.0f;
    float m_fovy = 45.0f;
    float m_distance_to_center = 1.0f;

    // Camera type
    CameraType m_type;
};
} // namespace mare

#endif