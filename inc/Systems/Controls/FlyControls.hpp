#ifndef FLYCONTROLS
#define FLYCONTROLS

// MARE
#include "Components/Rigidbody.hpp"
#include "Components/Transform.hpp"
#include "Mare.hpp"
#include "Systems.hpp"

namespace mare {

/**
 * @brief A ControlsSystem that operates on Transform and Rigidbody Components.
 * @details The FlyControls System will allow the user to change the view of on
 * Entity by using the mouse movements to look around and the W,A,S,and D keys
 * to move the Entity. Any Entity that uses this System must inherit from the
 * Transform and Rigidbody Components.
 *
 */
class FlyControls : public ControlsSystem<Transform, Rigidbody> {
  float speed = 1.0f;
  bool on_key(const RendererInput &input, Transform *transform,
              Rigidbody *rigidbody) override {
    if (rigidbody) {
      if (input.LEFT_SHIFT_PRESSED) {
        speed = 2.5f;
      } else {
        speed = 1.0f;
      }
      float x = 0.0f;
      float y = 0.0f;
      if (input.W_PRESSED) {
        y += 1.0f;
      }
      if (input.S_PRESSED) {
        y -= 1.0f;
      }
      if (input.D_PRESSED) {
        x += 1.0f;
      }
      if (input.A_PRESSED) {
        x -= 1.0f;
      }
      if (x || y) {
        x /= sqrtf(x * x + y * y);
        y /= sqrtf(x * x + y * y);
      }
      glm::vec3 dir = transform->get_forward_vector();
      glm::vec3 up = transform->get_up_vector();
      glm::vec3 right = glm::normalize(glm::cross(dir, up));
      glm::vec3 velocity = (dir * y + right * x) * speed;
      rigidbody->linear_velocity = velocity;
    }
    // pass though to next callback
    return false;
  }
  bool on_mouse_move(const RendererInput &input, Transform *transform,
                     Rigidbody *rigidbody) override {
    float sensitivity = 300.0f;
    float dtheta = float(input.mouse_vel.y) / sensitivity;
    float dphi = -float(input.mouse_vel.x) / sensitivity;
    glm::vec3 dir = transform->get_forward_vector();

    float theta = acosf(dir.z);
    float phi = atan2f(dir.y, dir.x);

    theta = glm::clamp(theta + dtheta, 0.01f, math::PI - 0.01f);
    phi += dphi;

    dir = glm::vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi),
                    cosf(theta));

    transform->face_towards(dir, {0.0f, 0.0f, 1.0f});
    return false;
  }
  bool on_mouse_button(const RendererInput &input, Transform *transform,
                       Rigidbody *rigidbody) override {
    return false;
  }
  bool on_mouse_wheel(const RendererInput &input, Transform *transform,
                      Rigidbody *rigidbody) override {
    return false;
  }
  bool on_resize(const RendererInput &input, Transform *transform,
                 Rigidbody *rigidbody) override {
    return false;
  }
};
} // namespace mare

#endif