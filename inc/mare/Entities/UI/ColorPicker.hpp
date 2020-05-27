#ifndef COLORPICKER
#define COLORPICKER

// MARE
#include "mare/Assets/Materials/VertexColorMaterial.hpp"
#include "mare/Assets/Meshes/ArrayMesh.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Components/Widget.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes.hpp"
#include "mare/Shader.hpp"
#include "mare/Systems/Rendering/PacketRenderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {

// Mesh
class ColorPickerMesh : public CompositeMesh {
public:
  ColorPickerMesh() {
    DrawMethod method = DrawMethod::TRIANGLES;
    // Hue Ring---------------------------------------------
    std::vector<float> ring_verts;
    std::vector<float> ring_colors;
    std::vector<uint32_t> ring_indes;
    int sides = 120;
    float thickness = 0.1f;
    float angle = 0.0f;
    float dtheta = 2.0f * math::PI / static_cast<float>(sides);
    for (int i = 0; i < sides + 1; i++) {
      ring_verts.push_back(0.5f * cos(angle));
      ring_verts.push_back(0.5f * sin(angle));
      glm::vec4 color = util::hsva_to_rgba({angle, 1.0f, 1.0f, 1.0f});
      ring_colors.push_back(color.r);
      ring_colors.push_back(color.g);
      ring_colors.push_back(color.b);
      ring_colors.push_back(color.a);
      angle += dtheta;
    }
    angle = 0.0f;
    for (int i = 0; i < sides + 1; i++) {
      ring_verts.push_back((0.5f + thickness) * cos(angle));
      ring_verts.push_back((0.5f + thickness) * sin(angle));
      glm::vec4 color = util::hsva_to_rgba({angle, 1.0f, 1.0f, 1.0f});
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
    std::vector<Referenced<Buffer<float>>> ring_vertex_buffers;
    ring_vertex_buffers.push_back(ring_vert_buffer);
    ring_vertex_buffers.push_back(ring_color_buffer);
    Referenced<SimpleMesh> ring_mesh =
        gen_ref<ArrayMesh>(method, ring_vertex_buffers, ring_index_buffer);

    // Color Triangle
    std::vector<float> tri_verts;
    std::vector<float> tri_colors;
    for (int i = 0; i < 3; i++) {
      float a = 2.0f * math::PI * (static_cast<float>(i) / 3.0f);
      float x = 0.5f * cosf(a);
      float y = 0.5f * sinf(a);
      tri_verts.push_back(x);
      tri_verts.push_back(y);
    }
    glm::vec4 color1 = util::hsva_to_rgba({0.0f, 1.0f, 1.0f, 1.0f});
    glm::vec4 color2 = util::hsva_to_rgba({0.0f, 0.0f, 1.0f, 1.0f});
    glm::vec4 color3 = util::hsva_to_rgba({0.0f, 1.0f, 0.0f, 1.0f});
    tri_colors.push_back(color1[0]);
    tri_colors.push_back(color1[1]);
    tri_colors.push_back(color1[2]);
    tri_colors.push_back(color1[3]);
    tri_colors.push_back(color2[0]);
    tri_colors.push_back(color2[1]);
    tri_colors.push_back(color2[2]);
    tri_colors.push_back(color2[3]);
    tri_colors.push_back(color3[0]);
    tri_colors.push_back(color3[1]);
    tri_colors.push_back(color3[2]);
    tri_colors.push_back(color3[3]);
    Referenced<Buffer<float>> tri_vert_buffer = Renderer::gen_buffer<float>(
        &tri_verts[0], sizeof(float) * tri_verts.size());
    Referenced<Buffer<float>> tri_color_buffer = Renderer::gen_buffer<float>(
        &tri_colors[0], sizeof(float) * tri_colors.size(),
        BufferType::WRITE_ONLY);
    tri_vert_buffer->set_format({{AttributeType::POSITION_2D, "position"}});
    tri_color_buffer->set_format({{AttributeType::COLOR, "color"}});
    std::vector<Referenced<Buffer<float>>> tri_vertex_buffers;
    tri_vertex_buffers.push_back(tri_vert_buffer);
    tri_vertex_buffers.push_back(tri_color_buffer);
    Referenced<SimpleMesh> tri_mesh =
        gen_ref<ArrayMesh>(method, tri_vertex_buffers);

    // Color picker
    std::vector<float> picker_verts;
    std::vector<float> picker_colors;
    std::vector<uint32_t> picker_indes;
    thickness = 0.01f;
    angle = 0.0f;
    dtheta = 2.0f * math::PI / 6.0f;
    float r = 0.025;
    for (int i = 0; i < 7; i++) {
      picker_verts.push_back(r * cos(angle));
      picker_verts.push_back(r * sin(angle));
      glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
      picker_colors.push_back(color.r);
      picker_colors.push_back(color.g);
      picker_colors.push_back(color.b);
      picker_colors.push_back(color.a);
      angle += dtheta;
    }
    angle = 0.0f;
    for (int i = 0; i < 7; i++) {
      picker_verts.push_back((r + thickness) * cos(angle));
      picker_verts.push_back((r + thickness) * sin(angle));
      glm::vec4 color = glm::vec4(1.0f);
      picker_colors.push_back(color.r);
      picker_colors.push_back(color.g);
      picker_colors.push_back(color.b);
      picker_colors.push_back(color.a);
      angle += dtheta;
    }
    for (int i = 0; i < 6; i++) {
      picker_indes.push_back(i);
      picker_indes.push_back(6 + i + 1);
      picker_indes.push_back(i + 1);
      picker_indes.push_back(i + 1);
      picker_indes.push_back(6 + i + 1);
      picker_indes.push_back(6 + i + 2);
    }
    Referenced<Buffer<float>> picker_vert_buffer = Renderer::gen_buffer<float>(
        &picker_verts[0], sizeof(float) * picker_verts.size());
    Referenced<Buffer<float>> picker_color_buffer = Renderer::gen_buffer<float>(
        &picker_colors[0], sizeof(float) * picker_colors.size());
    Referenced<Buffer<uint32_t>> picker_index_buffer =
        Renderer::gen_buffer<uint32_t>(&picker_indes[0],
                                       sizeof(float) * picker_indes.size());
    picker_vert_buffer->set_format({{AttributeType::POSITION_2D, "position"}});
    picker_color_buffer->set_format({{AttributeType::COLOR, "color"}});
    std::vector<Referenced<Buffer<float>>> picker_vertex_buffers;
    picker_vertex_buffers.push_back(picker_vert_buffer);
    picker_vertex_buffers.push_back(picker_color_buffer);
    Referenced<SimpleMesh> picker_mesh =
        gen_ref<ArrayMesh>(method, picker_vertex_buffers, picker_index_buffer);

    // Push Meshes
    push_mesh(ring_mesh);
    push_mesh(tri_mesh);
    push_mesh(picker_mesh);
    color_rgba = glm::vec4(2.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f);
  }
  void set_hue(float hue) {
    glm::vec4 color_hsva = util::rgba_to_hsva(color_rgba);
    color_hsva[0] = hue;
    color_rgba = util::hsva_to_rgba(color_hsva);
    // rotate triangle and picker
    get_meshes<Mesh>()[1]->set_rotation({0.0f, 0.0f, 1.0f}, hue);
    set_picker_rgba_color(color_rgba);
    // set color of hue vertex
    glm::vec4 vertex_color = util::hsva_to_rgba({hue,1.0f,1.0f,1.0f});
    get_meshes<SimpleMesh>()[1]->get_geometry_buffers()[1]->flush(
        &vertex_color[0], 0, sizeof(glm::vec4));
  }
  void set_picker_coords(glm::vec2 widget_coords) {
    glm::vec4 v1 = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
    glm::vec4 v2 = glm::vec4(0.5f * cos(math::TAU / 3.0f),
                             0.5f * sin(math::TAU / 3.0f), 0.0f, 1.0f);
    glm::vec4 v3 = glm::vec4(0.5f * cos(2.0f * math::TAU / 3.0f),
                             0.5f * sin(2.0f * math::TAU / 3.0f), 0.0f, 1.0f);
    v1 = get_meshes<Mesh>()[1]->get_rotation() * v1;
    v2 = get_meshes<Mesh>()[1]->get_rotation() * v2;
    v3 = get_meshes<Mesh>()[1]->get_rotation() * v3;
    glm::vec2 picker_pos =
        math::clamp_point_to_triangle(v1, v2, v3, widget_coords);
    get_meshes<Mesh>()[2]->set_position(glm::vec3(picker_pos, 0.0f));
    color_rgba = get_picker_rgba_color();
  }
  void set_picker_rgba_color(glm::vec4 rgba) {
    color_rgba = rgba;
    glm::vec4 hsva = util::rgba_to_hsva(rgba);
    float hue = hsva[0];
    // set position from hsva
    glm::vec2 s2 = hsva[2] * (v1 - v3) + v3;
    float b = s2.y - m * s2.x;
    glm::vec2 p2 = glm::vec2(s2.x + 1.0f, m * (s2.x + 1.0f) + b);
    glm::vec2 s1 = math::intersection(s2, p2, v3, v2);
    glm::vec2 local_pos = hsva[1]*(s2-s1) + s1;
    // set rotation about center from hue
    glm::vec2 position =
        glm::vec2(local_pos.x * cosf(hue) - local_pos.y * sinf(hue),
                  local_pos.x * sinf(hue) + local_pos.y * cosf(hue));
    set_picker_coords(position);
  }
  glm::vec4 get_picker_rgba_color() {
    glm::vec3 position = get_meshes<Mesh>()[2]->get_position();
    glm::vec4 color_hsva = util::rgba_to_hsva(color_rgba);
    float hue = color_hsva[0];
    // rotate picker position to zero hue
    float x = position[0] * cosf(-hue) - position[1] * sinf(-hue);
    float y = position[0] * sinf(-hue) + position[1] * cosf(-hue);
    float b = y - m * x;
    glm::vec2 p1 = glm::vec2(x, y);
    glm::vec2 p2 = glm::vec2(x + 1.0f, m * (x + 1.0f) + b);
    glm::vec2 s1 = math::intersection(p1, p2, v3, v2);
    glm::vec2 s2 = math::intersection(p1, p2, v3, v1);
    float V, S;
    if (isnanf(s1.x) || isnanf(s2.x) || glm::length(s2 - s1) == 0.0f) {
      V = 0.0f;
      S = 1.0f;
    } else {
      V = glm::length(s2 - v3) / s;
      S = glm::length(p1 - s1) / glm::length(s2 - s1);
    }
    return util::hsva_to_rgba({hue, S, V, color_rgba[3]});
  }

private:
  Referenced<Buffer<float>> vertex_buffer;
  Referenced<Buffer<float>> color_buffer;
  Referenced<Buffer<uint32_t>> index_buffer;
  glm::vec4 color_rgba;
  const glm::vec2 v1 = glm::vec2(0.5f, 0.0f);
  const glm::vec2 v2 =
      glm::vec2(0.5f * cos(math::TAU / 3.0f), 0.5f * sin(math::TAU / 3.0f));
  const glm::vec2 v3 = glm::vec2(0.5f * cos(2.0f * math::TAU / 3.0f),
                                 0.5f * sin(2.0f * math::TAU / 3.0f));
  const float s = cosf(math::PI / 6.0f);
  const float m = (v1.y - v2.y) / (v1.x - v2.x);
  const float h = s / sqrtf(2.0f);
};

// forward declaration
class ColorPickerControls;

/**
 * @brief A UI Element for Selecting Colors
 * @details A Widget with a glm::vec4 vlaue for an rgba color.
 */
class ColorPicker : public Entity, public Widget<glm::vec4>, public RenderPack {
public:
  ColorPicker(Layer *base_layer) : Widget(base_layer) {
    mesh = gen_ref<ColorPickerMesh>();
    mat = gen_ref<VertexColorMaterial>();
    push_packet({mesh, mat});
    gen_system<PacketRenderer>();
    gen_system<ColorPickerControls>();
    bounds.left() = -0.6f;
    bounds.right() = 0.6f;
    bounds.top() = 0.6f;
    bounds.bottom() = -0.6f;
    value = mesh->get_picker_rgba_color();
  }
  void set_hue(float hue) { mesh->set_hue(hue); }
  void set_picker_coords(glm::vec2 widget_coords) {
    mesh->set_picker_coords(widget_coords);
  }

  Referenced<ColorPickerMesh> mesh;
  Referenced<VertexColorMaterial> mat;
};
// Controls
class ColorPickerControls : public ControlsSystem<ColorPicker> {
public:
  /**
   * @brief If the left mouse button is pressed when pointed at the Widget, set
   * the focus to the base Layer and update the widget value.
   *
   * @param input The RendererInput from the Engine.
   * @param picker The UI widget.
   * @return true event is handeled.
   * @return false pass on event.
   */
  bool on_mouse_button(const RendererInput &input, ColorPicker *picker) {
    if (input.mouse_button == 1 && picker->is_in_bounds()) {
      Renderer::get_info().focus = picker->get_layer();
      glm::vec2 widget_coords = picker->get_widget_coords();
      glm::vec4 v1 = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
      glm::vec4 v2 = glm::vec4(0.5f * cos(math::TAU / 3.0f),
                               0.5f * sin(math::TAU / 3.0f), 0.0f, 1.0f);
      glm::vec4 v3 = glm::vec4(0.5f * cos(2.0f * math::TAU / 3.0f),
                               0.5f * sin(2.0f * math::TAU / 3.0f), 0.0f, 1.0f);
      v1 = picker->mesh->get_meshes<Mesh>()[1]->get_rotation() * v1;
      v2 = picker->mesh->get_meshes<Mesh>()[1]->get_rotation() * v2;
      v3 = picker->mesh->get_meshes<Mesh>()[1]->get_rotation() * v3;
      if (math::is_in_polygon(widget_coords,
                              {glm::vec2(v1), glm::vec2(v2), glm::vec2(v3)})) {
        focused_mesh = picker->mesh->get_meshes<Mesh>()[1];
      } else {
        focused_mesh = nullptr;
      }
      return on_mouse_move(input, picker);
    }
    Renderer::get_info().focus = nullptr;
    return false;
  }
  /**
   * @brief Use the mouse position to update the Widget's value if the Widget's
   * base Layer is focused.
   *
   * @param input The RendererInput from the Engine.
   * @param slider_ui The SliderUI.
   * @return true event is handeled.
   * @return false pass on event.
   */
  bool on_mouse_move(const RendererInput &input, ColorPicker *picker) {
    if (Renderer::get_info().focus == picker->get_layer()) {
      // model space coords of the widget
      glm::vec2 widget_coords = picker->get_widget_coords();
      // if focused mesh is triangle, move the picker to mouse position but
      // clamp to triangle
      if (focused_mesh == picker->mesh->get_meshes<Mesh>()[1]) {
        picker->set_picker_coords(widget_coords);
      } else {
        float hue = atan2f(widget_coords.y, widget_coords.x);
        picker->set_hue(hue);
      }
      picker->set_value(picker->mesh->get_picker_rgba_color());
      // event is handled
      return true;
    }
    return false;
  }

private:
  Mesh *focused_mesh = nullptr;
};
} // namespace mare

#endif