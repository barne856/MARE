#ifndef SWITCH
#define SWITCH

// MARE
#include "mare/Assets/Materials/BasicColorMaterial.hpp"
#include "mare/Assets/Materials/VertexColorMaterial.hpp"
#include "mare/Assets/Meshes/CircleMesh.hpp"
#include "mare/Assets/Meshes/QuadrangleMesh.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Components/Rigidbody.hpp"
#include "mare/Components/Widget.hpp"
#include "mare/Entities/UI/Slider.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes.hpp"
#include "mare/Systems/Rendering/PacketRenderer.hpp"

namespace mare {

// forward declare controls
class SwitchControls;
class SwitchPhysics;

/**
 * @brief A Switch Widget used to toggle settings.
 *
 */
class Switch : public Entity,
               public Widget<bool>,
               public RenderPack,
               public Rigidbody {
public:
  Switch(Layer *layer, util::Rect widget_bounds)
      : Widget(layer, widget_bounds) {
    value = false;

    left_circle_mesh = gen_ref<CircleMesh>(32, 0.5f);
    right_circle_mesh = gen_ref<CircleMesh>(32, 0.5f);
    knob_mesh = gen_ref<CircleMesh>(32, 0.35f);
    knob_shadow_mesh = gen_ref<KnobShadowMesh>(32, 0.45f, 0.15f);
    quad_mesh = gen_ref<QuadrangleMesh>();

    switch_material = gen_ref<BasicColorMaterial>();
    knob_material = gen_ref<BasicColorMaterial>();
    knob_shadow_material = gen_ref<VertexColorMaterial>();
    switch_material->set_color(switch_off_color);
    knob_material->set_color(knob_color);

    rescale();

    push_packet({left_circle_mesh, switch_material});
    push_packet({right_circle_mesh, switch_material});
    push_packet({quad_mesh, switch_material});
    push_packet({knob_shadow_mesh, knob_shadow_material});
    push_packet({knob_mesh, knob_material});

    gen_system<PacketRenderer>();
    gen_system<SwitchControls>();
    gen_system<SwitchPhysics>();
  }
  void on_focus() override { value = !value; }
  void rescale() override {
    float height = bounds.top() - bounds.bottom();
    float left = bounds.left() + 0.5f * height;
    float right = bounds.right() - 0.5f * height;

    left_circle_mesh->set_scale(glm::vec3(height));
    left_circle_mesh->set_position({left, 0.0f, 0.0f});
    right_circle_mesh->set_scale(glm::vec3(height));
    right_circle_mesh->set_position({right, 0.0f, 0.0f});
    knob_mesh->set_scale(glm::vec3(height));
    knob_mesh->set_position({left, 0.0f, 0.0f});
    quad_mesh->set_scale({right - left, height, 1.0f});

    glm::vec2 center = util::get_rect_center(bounds);
    left_circle_mesh->translate({center.x, center.y, 0.0f});
    right_circle_mesh->translate({center.x, center.y, 0.0f});
    knob_mesh->translate({center.x, center.y, 0.0f});
    quad_mesh->translate({center.x, center.y, 0.0f});

    knob_shadow_mesh->set_scale(knob_mesh->get_scale());
    knob_shadow_mesh->set_position(knob_mesh->get_position());

    off_position = glm::vec2(left + center.x, center.y);
    on_position = glm::vec2(right + center.x, center.y);
  }
  Referenced<CircleMesh> left_circle_mesh;
  Referenced<CircleMesh> right_circle_mesh;
  Referenced<CircleMesh> knob_mesh;
  Referenced<KnobShadowMesh> knob_shadow_mesh;
  Referenced<QuadrangleMesh> quad_mesh;
  Referenced<BasicColorMaterial> switch_material;
  Referenced<BasicColorMaterial> knob_material;
  Referenced<VertexColorMaterial> knob_shadow_material;
  glm::vec4 switch_off_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 switch_on_color{0.25f, 0.3f, 0.9f, 1.0f};
  glm::vec4 knob_color{0.77f, 0.77f, 0.85f, 1.0f};
  glm::vec2 on_position;
  glm::vec2 off_position;
};

class SwitchControls : public ControlsSystem<Switch> {
public:
  bool on_mouse_button(const RendererInput &input, Switch *sw) override {
    if (input.LEFT_MOUSE_JUST_PRESSED && sw->is_cursor_in_bounds()) {
      UIElement::focus(sw);
      return true;
    }
    return false;
  }
};

class SwitchPhysics : public PhysicsSystem<Switch> {
public:
  void update(float dt, Switch *sw) {
    glm::vec3 lerp_to =
        glm::vec3(sw->get_value() ? sw->on_position : sw->off_position, 0.0f);
    glm::vec3 position = sw->knob_mesh->get_position();
    if (position != lerp_to) {
      glm::vec3 force = k * (lerp_to - position) - c * sw->linear_velocity;
      sw->linear_velocity += force * dt;
      sw->knob_mesh->translate(sw->linear_velocity * dt);
      position = sw->knob_mesh->get_position();
      float dist = length(lerp_to - position);
      // if knob is within the tolerance distance, span to the lerp position.
      if (dist < tol) {
        sw->knob_mesh->set_position(lerp_to);
      }
      // if outside bounds, snap to lerp position.
      if (position.x > sw->on_position.x || position.x < sw->off_position.x) {
        sw->knob_mesh->set_position(lerp_to);
      }

      sw->knob_shadow_mesh->set_position(sw->knob_mesh->get_position());

      // lerp switch color
      float a = glm::clamp(dist / length(sw->on_position - sw->off_position),
                           0.0f, 1.0f);
      a = sw->get_value() ? a : 1.0f - a;
      sw->switch_material->set_color(
          glm::mix(sw->switch_on_color, sw->switch_off_color, a));

    } else {
      return;
    }
  }
  // Critically damped harmonic oscillator, mass = 1.
  const float k = 1000.0f; // strength of the spring.
  const float c =
      2.0f * sqrtf(k);      // strength of the damping (critically damped).
  const float tol = 0.001f; // distance when knob snaps to the lerp_to position.
};

} // namespace mare

#endif