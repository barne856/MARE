#ifndef TEXTBOX
#define TEXTBOX

// MARE
#include "mare/Assets/Materials/BasicColorMaterial.hpp"
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
 * @details Clicking on the text box will focus it. Once focused, the user can
 * input text from the keyboard. Backspace will delete the last character in the
 * box, enter will insert a new line. Ctrl-C will copy the boxes contents and
 * Ctrl-V will paste into the box.
 *
 */
class TextBox : public Entity, public Widget<std::string>, public RenderPack {
public:
  /**
   * @brief Construct a new TextBox
   *
   * @param layer The base layer where the Widget is attached.
   * @param widget_bounds The bounds in model space for the text box.
   * @param line_count The maximum number of lines in the text box. This will
   * determine the font size.
   * @param margin_thickness The thickness in model space of the margin space
   * around the text box.
   * @param boarder_thickness The thickness in model space of the boarder of the
   * text box.
   * @param size_in_bytes The maximum size in bytes allocated for the text in
   * the text box.
   */
  TextBox(Layer *layer, util::Rect widget_bounds, unsigned int line_count,
          float margin_thickness, float boarder_thickness,
          unsigned int max_strokes)
      : Widget(layer, widget_bounds), max_lines(line_count),
        margin_thickness(margin_thickness),
        boarder_thickness(boarder_thickness) {
    value = "";

    box = gen_ref<QuadrangleMesh>();
    boarder = gen_ref<QuadrangleMesh>();
    highlight = gen_ref<QuadrangleMesh>();
    text = gen_ref<CharMesh>("", 1.0f / 17.0f, 0.0f, max_strokes);

    text_material = gen_ref<BasicColorMaterial>();
    box_material = gen_ref<BasicColorMaterial>();
    boarder_material = gen_ref<BasicColorMaterial>();
    highlight_material = gen_ref<BasicColorMaterial>();

    text_material->set_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    box_material->set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    boarder_material->set_color(glm::vec4(0.0f, 0.0, 0.0f, 1.0f));
    highlight_material->set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    rescale();

    push_packet({boarder, boarder_material});
    push_packet({highlight, highlight_material});
    push_packet({box, box_material});
    push_packet({text, text_material});

    gen_system<PacketRenderer>();
    gen_system<TextBoxControls>();
  }
  /**
   * @brief Set the text of the text box.
   * @details The text will be wrapped and truncated to fit inside the text box.
   *
   * @param str The text to set.
   */
  void set_text(std::string str) {
    std::string wrapped_str = wrap_string(str);
    text->set_text(wrapped_str);
    value = wrapped_str;
  }
  /**
   * @brief Get the text of the text box as a string.
   *
   * @return The text inside of the text box.
   */
  std::string get_text() { return text->get_text(); }
  /**
   * @brief Clear the text box.
   *
   */
  void clear_text() { text->set_text(""); }
  /**
   * @brief Delete a character in the text.
   * @details The index to delete starts at zero and includes all control
   * characters in the string.
   *
   * @param index The index into the text to delete.
   */
  void delete_char(int index) {
    std::string str = text->get_text();
    int num_chars = str.length();
    if (num_chars > 0 && index < num_chars) {
      if (index == -1) {
        str.pop_back();
        text->set_text(str);
        value = str;
      } else {
        str.erase(index, 1);
        std::string wrapped_string = wrap_string(str);
        text->set_text(wrapped_string);
        value = wrapped_string;
      }
    }
  }
  /**
   * @brief Append a character to the text in the text box.
   * @details The text will be wrapped and truncated to fit inside the text box.
   *
   * @param character The character to append.
   */
  void append_char(char character) {
    std::string str = text->get_text();
    std::string no_lines_str = str;
    no_lines_str.erase(
        std::remove(no_lines_str.begin(), no_lines_str.end(), '\n'),
        no_lines_str.end());
    if (no_lines_str.length() < max_chars_per_line * max_lines) {
      std::stringstream str_stream(str);
      std::string line;
      int line_count = 0;
      while (str_stream.good()) {
        std::getline(str_stream, line, '\n');
        line_count += 1;
      }
      if (line_count == max_lines &&
          (character == '\n' || line.length() == max_chars_per_line)) {
        return;
      }
      str.push_back(character);
      std::string wrapped_str = wrap_string(str);
      text->set_text(wrapped_str);
      value = wrapped_str;
    }
  }
  /**
   * @brief This function sets the top left corner of the text box to a position
   * in the layer's world space.
   *
   * @param position The position to set the corner to.
   */
  void pin_top_left_corner(glm::vec2 position) {
    set_position({position.x - bounds.left(), position.y - bounds.top(), 0.0f});
  }
  /**
   * @brief This functions scales and positions of each meshes
   * that makes up the text box to the appropriate values for the bounds, margin
   * thickness, and boarder thicknesses.
   * @details The world space coordinates and scale for the entire textbox are
   * set through the text box's Transform Component or through the *pin*
   * functions of the text box.
   *
   */
  void rescale() override {
    float text_scale = (bounds.top() - bounds.bottom() -
                        2.0f * margin_thickness - 2.0f * boarder_thickness) /
                       static_cast<float>(max_lines);
    max_chars_per_line =
        floor(2.0f *
              (bounds.right() - bounds.left() - 2.0f * margin_thickness -
               2.0f * boarder_thickness) /
              text_scale);
    float box_width = (bounds.right() - bounds.left()) -
                      4.0f * boarder_thickness;
    float box_height = (bounds.top() - bounds.bottom()) -
                       4.0f * boarder_thickness;
    float highlight_width =
        bounds.right() - bounds.left() - 2.0f * boarder_thickness;
    float highlight_height =
        bounds.top() - bounds.bottom() - 2.0f * boarder_thickness;
    float boarder_width = (bounds.right() - bounds.left());
    float boarder_height = (bounds.top() - bounds.bottom());
    glm::vec2 box_center = glm::vec2((bounds.left() + bounds.right()) / 2.0f,
                                     (bounds.top() + bounds.bottom()) / 2.0f);
    box->set_scale({box_width, box_height, 1.0f});
    boarder->set_scale({boarder_width, boarder_height, 1.0f});
    highlight->set_scale({highlight_width, highlight_height, 1.0f});
    text->set_scale({text_scale, text_scale, 1.0f});
    box->set_position({box_center.x, box_center.y, 0.0f});
    boarder->set_position({box_center.x, box_center.y, 0.0f});
    highlight->set_position({box_center.x, box_center.y, 0.0f});
    text->set_position({bounds.left() + margin_thickness + 2.0f*boarder_thickness,
                        bounds.top() - margin_thickness - boarder_thickness,
                        0.0f});
  }
  void on_focus() override {
    highlight_material->set_color({0.25f, 0.3f, 0.9f, 1.0f});
  }
  void on_unfocus() override {
    highlight_material->set_color({1.0f, 1.0f, 1.0f, 1.0f});
  }
  Referenced<QuadrangleMesh> box;
  Referenced<QuadrangleMesh> boarder;
  Referenced<QuadrangleMesh> highlight;
  Referenced<CharMesh> text;
  float boarder_thickness;
  float margin_thickness;
  Referenced<BasicColorMaterial> text_material;
  Referenced<BasicColorMaterial> box_material;
  Referenced<BasicColorMaterial> boarder_material;
  Referenced<BasicColorMaterial> highlight_material;
  unsigned int max_lines;
  unsigned int max_chars_per_line;

private:
  /**
   * @brief Wraps the text to fit inside the text box
   *
   * @param str The string to wrap
   * @return The string with the necessary newline characters inserted to wrap
   * to string. The string will be trucated if it is too long or has too many
   * lines.
   */
  std::string wrap_string(std::string str) {
    std::string result{};
    std::stringstream str_stream(str);
    int line_count = 0;
    while (str_stream.good()) {
      std::string line;
      std::getline(str_stream, line, '\n');
      if (line.length() >= max_chars_per_line) {
        int num_lines = ceil(static_cast<float>(line.length()) /
                             static_cast<float>(max_chars_per_line));
        line_count += num_lines;
        for (int i = 1; i < num_lines; i++) {
          line.insert(max_chars_per_line * i + (i - 1), "\n");
        }
      } else {
        line_count += 1;
      }
      line += "\n";
      result += line;
    }
    // delete the first trailing newline character if it exists
    if (result.back() == '\n') {
      result.pop_back();
    }
    // delete lines that are too long
    while (line_count > max_lines) {
      while (result.back() != '\n' && !result.empty()) {
        result.pop_back();
      }
      result.pop_back();
      line_count -= 1;
    }
    return result;
  }
};

/**
 * @brief The controls for the TextBox Widget
 *
 */
class TextBoxControls : public ControlsSystem<TextBox> {
public:
  /**
   * @brief Focus the TextBox on click inside of Widget bounds. Unfocus TextBox
   * when clicked outside of Widget bounds.
   *
   * @param input The RendererInput
   * @param text_box The TextBox
   * @return true Handled
   * @return false Pass on
   */
  bool on_mouse_button(const RendererInput &input, TextBox *text_box) override {
    if (text_box->is_cursor_in_bounds()) {
      if (input.LEFT_MOUSE_JUST_PRESSED) {
        UIElement::focus(text_box);
      }
      return true;
    }
    return false;
  }
  bool on_mouse_move(const RendererInput &input, TextBox *text_box) override {
    return false;
  }
  /**
   * @brief Implementes copy/paste, backspace, and newline
   *
   * @param input The RendererInput
   * @param text_box The TextBox
   * @return true Handled
   * @return false Pass on
   */
  bool on_key(const RendererInput &input, TextBox *text_box) override {
    if (text_box->is_focused()) {
      // Control Characters
      if (input.BACKSPACE_PRESSED) {
        text_box->delete_char(-1);
      }
      if (input.ENTER_PRESSED) {
        text_box->append_char('\n');
      }
      if (input.LEFT_CONTROL_PRESSED && input.C_JUST_PRESSED) {
        Renderer::set_clipboard_string(text_box->get_text());
      }
      if (input.LEFT_CONTROL_PRESSED && input.V_JUST_PRESSED) {
        text_box->set_text(Renderer::get_clipboard_string());
      }
      return true;
    }
    return false;
  }
  /**
   * @brief Appends characters to the TextBox
   *
   * @param character The character to append.
   * @param text_box The TextBox
   * @return true Handled
   * @return false Pass on
   */
  bool on_char(char character, TextBox *text_box) override {
    if (text_box->is_focused()) {
      text_box->append_char(character);
      return true;
    }
    return false;
  }
};
} // namespace mare

#endif