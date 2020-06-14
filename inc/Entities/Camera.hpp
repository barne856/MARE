#ifndef CAMERA
#define CAMERA

// MARE
#include "Components/Transform.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {

/**
 * @brief The ProjectionType of the Camera.
 * @details Self explanitory.
 */
enum class ProjectionType { PERSPECTIVE, ORTHOGRAPHIC };

// forward declare camera system
class CameraControls;

/**
 * @brief A Camera Entity that is the base class for all Layers.
 * @details Cameras are the base class of all Layers. Since Camera's are
 * Entities they can be sued in the same way as other Entities and attached to
 * Scenes. If multiple Camera's are needed in a single Scene, the can be swaped
 * with the base class Camera using the Layer::swap_camera(Referenced<Camera>)
 * function.
 *
 * The Camera class provides functions to set the scale, clip planes, fovy, and
 * aspect of the Camera.
 */
class Camera : public Entity {
public:
  /**
   * @brief Construct a new Camera object
   * @details The ProjectionType for the Camera must be set upon construction.
   *
   * @param type The ProjectionType
   * @see ProjectionType
   */
  Camera(ProjectionType type) : type_(type) {
    recalculate_projection();
    gen_system<CameraControls>();
  }
  /**
   * @brief Destroy the Camera object
   */
  ~Camera() {}
  /**
   * @brief Recalculate the projection once some data is modified.
   */
  void recalculate_projection() {
    aspect_ = Renderer::get_info().window_aspect;
    switch (type_) {
    case ProjectionType::PERSPECTIVE:
      projection_ = glm::perspective(glm::radians(fovy_), aspect_, persp_near_,
                                     persp_far_);
      break;
    case ProjectionType::ORTHOGRAPHIC:
      projection_ =
          glm::ortho(-ortho_scale_ * aspect_, ortho_scale_ * aspect_,
                     -ortho_scale_, ortho_scale_, ortho_near_, ortho_far_);
      break;
    }
  }
  /**
   * @brief Set the ProjectionType of the Camera.
   *
   * @param type The ProjectionType
   * @see ProjectionType
   */
  void set_type(ProjectionType type) {
    type_ = type;
    recalculate_projection();
  }
  /**
   * @brief Set the vertical field of view of the Camera when the type is
   * ProjectionType::PERSPECTIVE. In degrees.
   *
   * @param fovy The vertical field of view to set. In degrees.
   */
  void set_fovy(float fovy) {
    fovy_ = fovy;
    recalculate_projection();
  }
  /**
   * @brief Set the vertical scale when the Camera's ProjectionType is
   * ProjectionType::ORTHOGRAPHIC.
   * @details If set to 1, the vertical coordinates of the camera will range
   * from -1 to 1 when the Camera is centered on 0. The horizontal scale is then
   * determined from the aspect ratio of the Camera.
   *
   * @param ortho_scale The scale to set.
   */
  void set_ortho_scale(float ortho_scale) {
    ortho_scale_ = ortho_scale;
    recalculate_projection();
  }
  /**
   * @brief Set the aspect ratio of the Camera.
   *
   * @param aspect The aspect ratio to set.
   */
  void set_aspect(float aspect) {
    aspect_ = aspect;
    recalculate_projection();
  }
  /**
   * @brief Set the near clip plane of the Orthographic Projection.
   *
   * @param near The distance to the near clip plane from the Camera.
   */
  void set_near_clip_plane_ortho(float near) {
    ortho_near_ = near;
    recalculate_projection();
  }
  /**
   * @brief Set the far clip plane of the Orthographic Projection.
   *
   * @param far The distance to the far clip plane from the Camera.
   */
  void set_far_clip_plane_ortho(float far) {
    ortho_far_ = far;
    recalculate_projection();
  }
  /**
   * @brief Set the near clip plane of the Perspective Projection
   * @details The near and far clip planes must be positive and the far clip
   * plane should be less than 1000 times the near clip plane to prevent errors
   * in the depth testing due to floating point precision.
   * @param near The distance to the near clip plane from the Camera.
   */
  void set_near_clip_plane_persp(float near) {
    persp_near_ = near;
    recalculate_projection();
  }
  /**
   * @brief Set the far clip plane of the Perspective Projection
   * @details The near and far clip planes must be positive and the far clip
   * plane should be less than 1000 times the near clip plane to prevent errors
   * in the depth testing due to floating point precision.
   * @param far The distance to the far clip plane from the Camera.
   */
  void set_far_clip_plane_persp(float far) {
    persp_far_ = far;
    recalculate_projection();
  }
  /**
   * @brief Get the ProjectionType of the Camera.
   *
   * @return The ProjectionType
   */
  ProjectionType get_type() const { return type_; }
  /**
   * @brief Get the projection matrix of the Camera.
   *
   * @return The projection matrix of the Camera.
   */
  glm::mat4 get_projection() const { return projection_; }
  /**
   * @brief Get the vertical field of view of the Camera with type
   * ProjectionType::PERSPECTIVE in units of degrees.
   *
   * @return The vertical field of view of the Camera with type
   * ProjectionType::PERSPECTIVE in units of degrees
   */
  float get_fovy() const { return fovy_; }
  /**
   * @brief Get the ortho scale of the Camera.
   *
   * @return the ortho scale of the Camera.
   * @see Camera::set_ortho_scale(float).
   */
  float get_ortho_scale() const { return ortho_scale_; }
  /**
   * @brief Get the aspect Camera.
   *
   * @return The aspect of the Camera.
   */
  float get_aspect() const { return aspect_; }
  /**
   * @brief Get the near clip plane of the Camera with type
   * ProjectionType::ORTHOGRAPHIC.
   *
   * @return The distance to the near clip plane from the Camera.
   */
  float get_near_clip_plane_ortho() const { return ortho_near_; }
  /**
   * @brief Get the far clip plane of the Camera with type
   * ProjectionType::ORTHOGRAPHIC.
   *
   * @return The distance to the far clip plane from the Camera.
   */
  float get_far_clip_plane_ortho() const { return ortho_far_; }
  /**
   * @brief Get the near clip plane of the Camera with type
   * ProjectionType::PERSPECTIVE.
   *
   * @return The distance to the near clip plane from the Camera.
   */
  float get_near_clip_plane_persp() const { return persp_near_; }
  /**
   * @brief Get the far clip plane of the Camera with type
   * ProjectionType::PERSPECTIVE.
   *
   * @return The distance to the far clip plane from the Camera.
   */
  float get_far_clip_plane_persp() const { return persp_far_; }

private:
  glm::mat4 projection_; /**< The projection matrix of the Camera.*/
  float fovy_ = 45.0f;   /**< The vertical field of view of the Camera with type
                            ProjectionType::PERSPECTIVE.*/
  float ortho_scale_ = 1.0f; /**< The scale of the Camera's view with type
                                ProjectionType::ORTHOGRAPHIC.*/
  float aspect_ = 1.0f;      /**< The aspect of the Camera's view.*/
  float ortho_near_ = -1.0f; /**< The distance to the near clip plane of the
                                Camera using ProjectionType::ORTHOGRAPHIC.*/
  float ortho_far_ = 1.0f; /**< The distance to the far clip plane of the Camera
                 using ProjectionType::ORTHOGRAPHIC.*/
  float persp_near_ = 0.1f; /**< The distance to the near clip plane of the
                 Camera using ProjectionType::PERSPECTIVE.*/
  float persp_far_ = 10.0f; /**< The distance to the far clip plane of the
                 Camera using ProjectionType::PERSPECTIVE.*/

  ProjectionType type_; /**< The ProjectionType of the Camera.*/
};

/**
 * @brief Simple controls for the Camera that will automatically update the
 * projection when the window changes aspect.
 */
class CameraControls : public ControlsSystem<Camera> {
  bool on_resize(const RendererInput &input, Camera *camera_entity) override {
    // recalcualte the projection with the new aspect
    camera_entity->set_aspect(Renderer::get_info().window_aspect);
    camera_entity->recalculate_projection();
    return false; // continue
  }
};

} // namespace mare

#endif