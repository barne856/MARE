#ifndef TEXTBOX
#define TEXTBOX

// MARE
#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Assets/Meshes/CharMesh.hpp"
#include "mare/Assets/Meshes/QuadrangleMesh.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Components/Widget.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes.hpp"
#include "mare/Systems.hpp"
#include "mare/Systems/Rendering/PacketRenderer.hpp"

// Standard Library
#include <sstream>
#include <string>

namespace mare {

// forward declare systems
class TextBoxControls;

/**
 * @brief Provides a text box UI element that the user can use to enter text
 *
 */
class TextBox : public Entity, public Widget<std::string>, public RenderPack {
public:
  /**
   * @brief Construct a new TextBox UI element.
   *
   * @param layer The base layer for the widget.
   */
  TextBox(Layer *layer, util::Rect bounds, util::Rect margins,
          unsigned int line_count, uint32_t size_in_bytes)
      : Widget(layer), max_lines(line_count), bounds(bounds), margins(margins) {
    float scale =
        (bounds.top() - bounds.bottom()) / static_cast<float>(line_count);
    max_chars_per_line = floor(2.0f * (bounds.right() - bounds.left()) / scale);

    box = gen_ref<QuadrangleMesh>(margins);
    decoration = gen_ref<QuadrangleMesh>(margins);
    text = gen_ref<CharMesh>("", size_in_bytes);

    text_material = gen_ref<BasicMaterial>();
    box_material = gen_ref<BasicMaterial>();
    decoration_material = gen_ref<BasicMaterial>();

    text->set_position({bounds.left(), bounds.top(), 0.0f});
    text->set_scale(glm::vec3(scale / 2.0f, scale, 1.0f));

    text_material->set_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    box_material->set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    decoration_material->set_color(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

    float x = 1.0f + decoration_thickness / (margins.right() - margins.left());
    float y = 1.0f + decoration_thickness / (margins.top() - margins.bottom());
    decoration->set_scale({x, y, 1.0f});

    push_packet({decoration, decoration_material});
    push_packet({box, box_material});
    push_packet({text, text_material});

    gen_system<PacketRenderer>();
    gen_system<TextBoxControls>();
  }
  void set_text(std::string str) { text->set_text(wrap_string(str)); }
  void pin_top_left_corner(glm::vec2 position) {
    set_position(
        {position.x - margins.left(), position.y - margins.top(), 0.0f});
  }
  Referenced<QuadrangleMesh> box;
  Referenced<QuadrangleMesh> decoration;
  Referenced<CharMesh> text;
  float decoration_thickness = 0.01;
  Referenced<BasicMaterial> text_material;
  Referenced<BasicMaterial> box_material;
  Referenced<BasicMaterial> decoration_material;
  unsigned int max_lines;
  unsigned int max_chars_per_line;
  util::Rect bounds;
  util::Rect margins;

private:
  /**
   * @brief Wraps the text to fit inside the text box
   *
   * @param str The string to wrap
   * @return The string with the necessary newline characters inserted to wrap
   * to string
   */
  std::string wrap_string(std::string str) {
    std::string result{};
    std::stringstream str_stream(str);
    while (str_stream.good()) {
      std::string line;
      std::getline(str_stream, line, '\n');
      int num_lines = line.length() / max_chars_per_line;
      for (int i = 1; i <= num_lines; i++) {
        line.insert(max_chars_per_line * i + (i - 1), "\n");
      }
      result += line += '\n';
    }
    result.pop_back();
    return result;
  }
};

/**
 * @brief The controls for the TextBox Widget
 *
 */
class TextBoxControls : public ControlsSystem<TextBox> {
public:
  bool on_mouse_button(const RendererInput& input, TextBox *text_box) override
  {
    
  }
  bool on_char(unsigned char character, TextBox *text_box) override { return false; }
};
} // namespace mare

#endif