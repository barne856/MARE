#ifndef ORBITCONTROLS
#define ORBITCONTROLS

// MARE
#include "Components/Transform.hpp"
#include "Renderer.hpp"
#include "Systems.hpp"

// External Libraries
#include <gtx/rotate_vector.hpp> // rotateZ

namespace mare {

/**
 * @brief A ControlsSystem that operates on a Transform Component
 * @details The OrbitControls System will allow to user to control an Entity by
 * orbiting around a point when the user clicks and drags with the left mouse
 * button. Any Entity that uses this System must inherit from a Transform
 * Component.
 *
 * Using this system will orient the Camera/Entity such that the horizontal
 * plane is x-y.
 *
 */
class OrbitControls : public ControlsSystem<Transform> {
  public:
  float distance_to_center = 1.0f;
  float orbit_sensitivity = 3.0f;
  float translational_sensitivity = 1.0f;
  float zoom_sensitvity = 0.1f;
  float minimum_distance_to_center = 100.0f;
  float maximum_distance_to_center = 20000.0f;
  bool on_mouse_move(const RendererInput &input,
                     Transform *transform) override {
    if(input.MIDDLE_MOUSE_PRESSED)
    {
      glm::vec3 dir = transform->get_forward_vector();
      glm::vec3 pos = transform->get_position();
      glm::vec3 center = pos + dir * distance_to_center;
      float dist_2D = glm::length(glm::vec2(center - pos));
      float angle = atan2f(dir.y, dir.x) - math::PI/2.0f;
      glm::vec3 direction{};
      direction.x = 2.0f * static_cast<float>(input.mouse_vel.x) / static_cast<float>(Renderer::get_info().window_height);
      direction.y = -2.0f * static_cast<float>(input.mouse_vel.y) / static_cast<float>(Renderer::get_info().window_height);
      direction.z = 0.0f;
      if(dist_2D < 0.1f)
      {
        glm::vec3 up = transform->get_up_vector();
        angle = atan2f(up.y, up.x) - math::PI/2.0f;
      }
      else if(dir.z > 0.0f)
      {
        direction.y *= -1.0f;
      }
      transform->translate(-0.5f * distance_to_center * translational_sensitivity * glm::rotateZ(direction, angle));
    }
    else if (input.LEFT_MOUSE_PRESSED) {
      // orbit around center and adjust inclination

      float dtheta = -orbit_sensitivity*static_cast<float>(input.mouse_vel.y) / static_cast<float>(Renderer::get_info().window_height);
      float dphi = -orbit_sensitivity*static_cast<float>(input.mouse_vel.x) / static_cast<float>(Renderer::get_info().window_height);
      glm::vec3 dir = transform->get_forward_vector();
      glm::vec3 pos = transform->get_position();
      glm::vec3 up = {0.0f, 0.0f, 1.0f};

      glm::vec3 center = pos + dir * distance_to_center;
      float r_xy =
          glm::length(glm::vec2(pos.x, pos.y) - glm::vec2(center.x, center.y));
      float phi = atan2f((pos.y - center.y), (pos.x - center.x));
      float theta;
      if (r_xy != 0.0f) {
        theta = math::PI / 2.0f - atan2f(pos.z - center.z, r_xy);
      } else {
        theta = 0.0f;
      }
      float r = glm::length(pos - center);
      if (r_xy == 0.0f || theta + dtheta <= 0.0f) {
        pos.x = center.x;
        pos.y = center.y;
        pos.z = r + center.z;
        up = transform->get_up_vector();
        phi = atan2f(up.y, up.x) - math::PI;
        up = {cosf(phi + dphi + math::PI), sinf(phi + dphi + math::PI), 0.0f};
      }
      if (r_xy == 0.0f && theta + dtheta >= math::PI) {
        pos.x = center.x;
        pos.y = center.y;
        pos.z = -r + center.z;
        up = transform->get_up_vector();
        phi = atan2f(up.y, up.x) - math::PI;
        up = {cosf(phi + dphi + math::PI), sinf(phi + dphi + math::PI), 0.0f};
      }
      if (theta + dtheta > 0.0f && theta + dtheta < math::PI) {
        pos.x = r * sin(theta + dtheta) * cos(phi + dphi) + center.x;
        pos.y = r * sin(theta + dtheta) * sin(phi + dphi) + center.y;
        pos.z = r * cos(theta + dtheta) + center.z;
      }
      transform->set_position(pos);
      transform->face_towards(center, up);
      return true;
    }
    return false;
  }
  bool on_key(const RendererInput &input, Transform *transform) override {
    return false;
  }
  bool on_mouse_button(const RendererInput &input,
                       Transform *transform) override {
    return false;
  }
  bool on_mouse_wheel(const RendererInput &input,
                      Transform *transform) override {
    glm::vec3 dir = transform->get_forward_vector();
    glm::vec3 pos = transform->get_position();
    glm::vec3 center = pos + dir * distance_to_center;
    distance_to_center = glm::clamp(distance_to_center - input.mouse_scroll * zoom_sensitvity, minimum_distance_to_center, maximum_distance_to_center);
    transform->set_position({center - dir * distance_to_center});
    return false;
  }
  bool on_resize(const RendererInput &input, Transform *transform) override {
    return false;
  }
}; // namespace mare
} // namespace mare

#endif