#ifndef ORBITCONTROLS
#define ORBITCONTROLS

// MARE
#include "mare/Components/Transform.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

/**
 * @brief A ControlsSystem that operates on a Transform Component
 * @details The OrbitControls System will allow to user to control an Entity by
 * orbiting around a point when the user clicks and drags with the left mouse
 * button. Any Entity that uses this System must inherit from a Transform
 * Component.
 *
 */
class OrbitControls : public ControlsSystem<Transform> {
  float distance_to_center = 1.0f;
  bool on_mouse_move(const RendererInput &input,
                     Transform *transform) override {
    if (input.mouse_button == 1) {
      // orbit around center and adjust inclination
      float sensitivity = 300.0f;
      float dtheta = -float(input.mouse_vel.y) / sensitivity;
      float dphi = -float(input.mouse_vel.x) / sensitivity;
      glm::vec3 dir = transform->get_direction();
      glm::vec3 pos = transform->get_position();
      glm::vec3 up = transform->get_up_vector();

      glm::vec3 center = pos + dir * distance_to_center;

      float r_xy =
          glm::length(glm::vec2(pos.x, pos.y) - glm::vec2(center.x, center.y));
      if (r_xy == 0.0f) {
        float phi = math::PI / 2.0f;

        if (up.x > 0.0f) {
          phi = atan((up.y) / (up.x));
        } else if (up.x < 0.0f) {
          phi = atan((up.y) / (up.x)) + math::PI;
        } else if (up.y < 0.0f) {
          phi = -math::PI / 2.0f;
        }
        up = {cos(phi + dphi), sin(phi + dphi), 0.0f};
        if (dtheta > 0.0f) {
          up = {0.0f, 0.0f, 1.0f};
          float r = glm::length(pos - center);
          pos.x = r * sin(dtheta) * cos(phi + dphi + math::PI) + center.x;
          pos.y = r * sin(dtheta) * sin(phi + dphi + math::PI) + center.y;
          pos.z = r * cos(dtheta) + center.z;
        }
      } else {
        float phi = math::PI;

        if (pos.x - center.x > 0.0f) {
          phi = atan((pos.y - center.y) / (pos.x - center.x));
        } else if (pos.x - center.x < 0.0f) {
          phi = atan((pos.y - center.y) / (pos.x - center.x)) + math::PI;
        } else if (pos.y - center.y < 0.0f) {
          phi = -math::PI / 2.0f;
        }
        float r = glm::length(pos - center);
        float h = pos.z - center.z;
        float theta = math::PI / 2.0f - atan(h / r_xy);

        if (theta + dtheta > math::PI / 2.0f) {
          dtheta = math::PI / 2.0f - theta;
        }
        if (theta + dtheta < 0.0f) {
          dtheta = -theta;
          up = {dir.x, dir.y, 0.0f};
        }
        pos.x = r * sin(theta + dtheta) * cos(phi + dphi) + center.x;
        pos.y = r * sin(theta + dtheta) * sin(phi + dphi) + center.y;
        pos.z = r * cos(theta + dtheta) + center.z;
      }
      transform->set_position(pos);
      transform->face_towards((center - pos));
      transform->set_up_vector(up);
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
    return false;
  }
  bool on_resize(const RendererInput &input, Transform *transform) override {
    return false;
  }
};
} // namespace mare

#endif