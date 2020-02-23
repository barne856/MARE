#ifndef CAMERA
#define CAMERA

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Controls.hpp"

#include "ext/matrix_transform.hpp"  // glm::translate, glm::rotate, glm::scale
#include "ext/matrix_clip_space.hpp" // glm::perspective

namespace mare
{

enum class ProjectionType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera
{
public:
    Camera(ProjectionType type);
    ~Camera();

    // Get projection and view matrices from the camera
    glm::mat4 projection() const;
    glm::mat4 view() const;

    // Set the controls for the camera
    void set_controls(Scoped<Controls> controls);

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

private:
    // Camera Controls
    Scoped<Controls> controls_;

    // Camera Properties
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::vec3 linear_velocity_;
    float angular_velocity_;
    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 up_;
    float scale_ = 1.0f;
    float aspect_ = 1.0f;
    float near_ = -1.0f;
    float far_ = 1.0f;
    float fovy_ = 45.0f;
    float distance_to_center_ = 1.0f;

    // Projection type
    ProjectionType type_;
};
} // namespace mare

#endif