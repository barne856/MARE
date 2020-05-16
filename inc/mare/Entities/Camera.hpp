#ifndef CAMERA
#define CAMERA

// MARE
#include "mare/Entity.hpp"
#include "mare/Renderer.hpp"
#include "mare/Components/Rigidbody.hpp"
#include "mare/Systems/Physics/EulerMethod.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{

    enum class ProjectionType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    // forward declare camera system
    class CameraControls;

    class Camera : public Entity, public Rigidbody
    {
    public:
        Camera(ProjectionType type) : type_(type), eye_(glm::vec3(0.0f)), direction_(glm::vec3(0.0f, 0.0f, -1.0f)), up_(glm::vec3(0.0f, 1.0f, 0.0f))
        {
            if (type == ProjectionType::PERSPECTIVE)
            {
                near_ = 0.1f;
                far_ = 10.0f;
            }
            set_forward_vector(direction_);
            recalculate_projection();
            gen_system<CameraControls>();
            gen_system<EulerMethod>();
        }
        ~Camera() {}
        void recalculate_projection()
        {
            aspect_ = Renderer::get_info().window_aspect;
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
        // Set camera properties
        void set_fov(float fovy)
        {
            fovy_ = fovy;
            recalculate_projection();
        }
        void set_scale(float scale)
        {
            scale_ = scale;
            recalculate_projection();
        }
        void set_aspect(float aspect)
        {
            aspect_ = aspect;
            recalculate_projection();
        }
        void set_near_clip_plan(float near)
        {
            near_ = near;
            recalculate_projection();
        }
        void set_far_clip_plan(float far)
        {
            far_ = far;
            recalculate_projection();
        }
        // Get projection and view matrices from the camera
        glm::mat4 projection() const { return projection_; }
        glm::mat4 view() const { return get_view(); }
        float aspect() const { return aspect_; }

    private:
        // Camera Properties
        glm::mat4 projection_;
        glm::vec3 eye_;
        glm::vec3 direction_;
        glm::vec3 up_;
        float scale_ = 1.0f;
        float aspect_ = 1.0f;
        float near_ = -1.0f;
        float far_ = 1.0f;
        float fovy_ = 45.0f;

        // Projection type
        ProjectionType type_;
    };

    class CameraControls : public ControlsSystem<Camera>
    {
        bool on_resize(const RendererInput &input, Camera *camera_entity) override
        {
            // recalcualte the projection with the new aspect
            camera_entity->set_aspect(Renderer::get_info().window_aspect);
            camera_entity->recalculate_projection();
            return false; // continue
        }
    };

} // namespace mare

#endif