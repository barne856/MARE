#ifndef SLIDER
#define SLIDER

// MARE
#include "Materials/BasicColorMaterial.hpp"
#include "Materials/VertexColorMaterial.hpp"
#include "Meshes/CircleMesh.hpp"
#include "Meshes/QuadrangleMesh.hpp"
#include "Components/RenderPack.hpp"
#include "Components/Widget.hpp"
#include "Entity.hpp"
#include "Meshes.hpp"
#include "Renderer.hpp"
#include "Systems.hpp"
#include "Systems/Rendering/PacketRenderer.hpp"

namespace mare {

// forward declare Slider components
class SliderRenderer;
class SliderControls;

class KnobShadowMesh : public SimpleMesh {
public:
  KnobShadowMesh(int sides, float outer_radius, float inner_radius) {
    set_draw_method(DrawMethod::TRIANGLES);
    // Ring---------------------------------------------
    std::vector<float> ring_verts;
    std::vector<float> ring_colors;
    std::vector<uint32_t> ring_indes;
    float thickness = outer_radius - inner_radius;
    float angle = 0.0f;
    float dtheta = 2.0f * math::PI / static_cast<float>(sides);
    for (int i = 0; i < sides + 1; i++) {
      ring_verts.push_back(inner_radius * cos(angle));
      ring_verts.push_back(inner_radius * sin(angle));
      glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f};
      ring_colors.push_back(color.r);
      ring_colors.push_back(color.g);
      ring_colors.push_back(color.b);
      ring_colors.push_back(color.a);
      angle += dtheta;
    }
    angle = 0.0f;
    for (int i = 0; i < sides + 1; i++) {
      ring_verts.push_back((inner_radius+ thickness) * cos(angle));
      ring_verts.push_back((inner_radius + thickness) * sin(angle));
      glm::vec4 color = {0.0f, 0.0f, 0.0f, 0.0f};
      ring_colors.push_back(color.r);
      ring_colors.push_back(color.g);
      ring_colors.push_back(color.b);
      ring_colors.push_back(color.a);
      angle += dtheta;
    }
    for (int i = 0; i < sides; i++) {
      ring_indes.push_back(i);
      ring_indes.push_back(sides + i + 1);
      ring_indes.push_back(i + 1);
      ring_indes.push_back(i + 1);
      ring_indes.push_back(sides + i + 1);
      ring_indes.push_back(sides + i + 2);
    }
    Referenced<Buffer<float>> ring_vert_buffer = Renderer::gen_buffer<float>(
        &ring_verts[0], sizeof(float) * ring_verts.size());
    Referenced<Buffer<float>> ring_color_buffer = Renderer::gen_buffer<float>(
        &ring_colors[0], sizeof(float) * ring_colors.size());
    Referenced<Buffer<uint32_t>> ring_index_buffer =
        Renderer::gen_buffer<uint32_t>(&ring_indes[0],
                                       sizeof(float) * ring_indes.size());
    ring_vert_buffer->set_format({{AttributeType::POSITION_2D, "position"}});
    ring_color_buffer->set_format({{AttributeType::COLOR, "color"}});
    add_geometry_buffer(ring_vert_buffer);
    add_geometry_buffer(ring_color_buffer);
    set_index_buffer(ring_index_buffer);
  }
};

/**
 * @brief A simple UI element that provides a slider bar and contains a `float`
 * value.
 */
class Slider : public Entity, public Widget<float>, public RenderPack {
public:
  /**
   * @brief Construct a new SliderUI object.
   *
   * @param layer The base Layer that this UI element is on.
   */
  Slider(Layer *layer, util::Rect widget_bounds)
      : Widget(layer, widget_bounds) {
    value = 0.0f;

    fill_mesh = gen_ref<QuadrangleMesh>();
    bar_mesh = gen_ref<QuadrangleMesh>();
    knob_mesh = gen_ref<CircleMesh>(32, 0.5f);
    knob_shadow_mesh = gen_ref<KnobShadowMesh>(32, 0.6f, 0.24f);
    left_circle_mesh = gen_ref<CircleMesh>(32, 0.5f);
    right_circle_mesh = gen_ref<CircleMesh>(32, 0.5f);

    slider_material = gen_ref<BasicColorMaterial>();
    fill_material = gen_ref<BasicColorMaterial>();
    knob_material = gen_ref<BasicColorMaterial>();
    knob_shadow_material = gen_ref<VertexColorMaterial>();
    slider_material->set_color(slider_color);
    fill_material->set_color(fill_color);
    knob_material->set_color(knob_color);

    // use bounds to set the scale
    rescale();

    push_packet({left_circle_mesh, fill_material});
    push_packet({right_circle_mesh, slider_material});
    push_packet({fill_mesh, fill_material});
    push_packet({bar_mesh, slider_material});
    push_packet({knob_shadow_mesh, knob_shadow_material});
    push_packet({knob_mesh, knob_material});

    // push systems
    gen_system<PacketRenderer>();
    gen_system<SliderControls>();
    gen_system<SliderRenderer>();
  }
  void rescale() override {
    float height = bounds.top() - bounds.bottom();
    float left = bounds.left() + 0.5f * height;
    float right = bounds.right() - 0.5f * height;

    left_circle_mesh->set_scale(glm::vec3(height / 1.5f));
    left_circle_mesh->set_position({left, 0.0f, 0.0f});
    right_circle_mesh->set_scale(glm::vec3(height / 1.5f));
    right_circle_mesh->set_position({right, 0.0f, 0.0f});
    knob_mesh->set_scale(glm::vec3(height));
    knob_mesh->set_position({left + value * (right - left), 0.0f, 0.0f});
    bar_mesh->set_scale({(1.0f - value) * (right - left), height / 1.5f, 1.0f});
    fill_mesh->set_scale({value * (right - left), height / 1.5f, 1.0f});

    glm::vec2 center = util::get_rect_center(bounds);
    left_circle_mesh->translate({center.x, center.y, 0.0f});
    right_circle_mesh->translate({center.x, center.y, 0.0f});
    knob_mesh->translate({center.x, center.y, 0.0f});
    fill_mesh->set_position(
        {left + 0.5f * value * (right - left) + center.x, center.y, 0.0f});
    bar_mesh->set_position(
        {right - 0.5f * (1.0f - value) * (right - left) + center.x, center.y,
         0.0f});

    knob_shadow_mesh->set_position(knob_mesh->get_position());
    knob_shadow_mesh->set_scale(knob_mesh->get_scale());
  }
  /**
   * @brief Convert the horizontal coordinate in model space to a value for the
   * slider.
   *
   * @param x The horizontal coordinate of position in the Slider's model space.
   * @return The value.
   */
  float position_to_value(float x) {
    float height = bounds.top() - bounds.bottom();
    float left = bounds.left() + 0.5f * height;
    float right = bounds.right() - 0.5f * height;
    return glm::clamp((x - left) / (right - left), 0.0f, 1.0f);
  }
  Referenced<QuadrangleMesh> fill_mesh;
  Referenced<QuadrangleMesh> bar_mesh;
  Referenced<CircleMesh> knob_mesh;
  Referenced<KnobShadowMesh> knob_shadow_mesh;
  Referenced<CircleMesh> left_circle_mesh;
  Referenced<CircleMesh> right_circle_mesh;
  Referenced<BasicColorMaterial>
      slider_material; /**< The material used to render the slider background
                          and knob.*/
  Referenced<BasicColorMaterial>
      fill_material; /**< The material used to render the slider background.*/
  Referenced<BasicColorMaterial> knob_material;
  Referenced<VertexColorMaterial> knob_shadow_material;
  glm::vec4 slider_color{
      1.0f, 1.0f, 1.0f,
      1.0f}; /**< The color of the bacground and knob for the slider.*/
  glm::vec4 fill_color{0.25f, 0.3f, 0.9f,
                       1.0f}; /**< The color of the fill for the slider.*/
  glm::vec4 knob_color{0.77f, 0.77f, 0.85f, 1.0f};
};

class SliderRenderer : public RenderSystem<Slider> {
public:
  void render(float dt, Camera *camera, Slider *slider) {
    if (slider->is_focused()) {
      a = glm::clamp(a + speed * dt, 0.0f, 1.0f);
    } else {
      a = glm::clamp(a - speed * dt, 0.0f, 1.0f);
    }

    glm::vec4 knob_color = glm::mix(slider->knob_color, slider->fill_color, a);
    slider->knob_material->set_color(knob_color);
  }
  float a = 0.0f;
  float speed = 10.0f;
};

/**
 * @brief A System used to control the SliderUI
 */
class SliderControls : public ControlsSystem<Slider> {
public:
  /**
   * @brief If the left mouse button is pressed when pointed at the slider, set
   * the focus to the base Layer and move the knob to the pointed location which
   * will change the value of the SliderUI Widget.
   *
   * @param input The RendererInput from the Engine.
   * @param slider_ui The SliderUI.
   * @return true event is handeled.
   * @return false pass on event.
   */
  bool on_mouse_button(const RendererInput &input, Slider *slider_ui) {
    if (input.LEFT_MOUSE_JUST_PRESSED && slider_ui->is_cursor_in_bounds()) {
      UIElement::focus(slider_ui);
      return on_mouse_move(input, slider_ui);
    }
    return false;
  }
  /**
   * @brief Move the slider knob to the mouse position and adjust the Widget's
   * value if the base Layer is focused.
   *
   * @param input The RendererInput from the Engine.
   * @param slider_ui The SliderUI.
   * @return true event is handeled.
   * @return false pass on event.
   */
  bool on_mouse_move(const RendererInput &input, Slider *slider_ui) {
    if (slider_ui->is_focused()) {
      glm::vec2 relative_position = slider_ui->get_model_coords();
      slider_ui->set_value(slider_ui->position_to_value(relative_position.x));
      slider_ui->rescale();
      // event is handled
      return true;
    }
    return false;
  }
};

} // namespace mare

#endif