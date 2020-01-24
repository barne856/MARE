//================================================================================
//
// DATE MODIFIED: 11/26/2018
// AUTHOR: BRENDAN BARNES
//
//================================================================================
// Creates a renderable Text object for an OpenGL context.
//
// The text object will create renderable text for an OpenGL context from
// a string consisting of ASCII characters (excluding control characters
// that are not the new line character). If a new line character is
// encountered, the text will be arranged as expected. The 'Keying' variable
// is the spaceing between letters (default is 1.0).
//
//   DETAILS:
//   Each of the ASCII characters excluding the control characters have
//   been constructed out of lines on a 17 x 17 grid. The ASCII_font is
//   a collection of the indicies defining each line for each character
//   Each line is made from 4 indices (X1,Y1--X2,Y2). Each letter fits
//   within a 1x1 square in the bottom right quadrant of the screen space.
//   Default setting are applied to change the scale and position of the
//   text from this initial model space.
//
//   There is a buffer of 2 indices on all edges of the letters to include
//   spacing of lines and letters. On occasion this buffer may be broken for some
//   special case characters.
//
//            _0__1__2__3__4__5__6__7__8__9__10_11_12_13_14_15_16
//        0   |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__| (1,0)
//        1   |__|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|__|
//        2   |__|xx|__|__|__|__|__|__||||__|__|__|__|__|__|xx|__|
//        3   |__|xx|__|__|__|__|__|__||||__|__|__|__|__|__|xx|__|
//        4   |__|xx|__|__|__|__|__|//|__|\\|__|__|__|__|__|xx|__|
//        5   |__|xx|__|__|__|__|__|//|__|\\|__|__|__|__|__|xx|__|
//        6   |__|xx|__|__|__|__|//|__|__|__|\\|__|__|__|__|xx|__|
//        7   |__|xx|__|__|__|__|//|__|__|__|\\|__|__|__|__|xx|__|
//        8   |__|xx|__|__|__|//|__|__|__|__|__|\\|__|__|__|xx|__|
//        9   |__|xx|__|__|__|//|__|__|__|__|__|\\|__|__|__|xx|__|
//        10  |__|xx|__|__|//|==|==|==|==|==|==|==|\\|__|__|xx|__|
//        11  |__|xx|__|__|//|__|__|__|__|__|__|__|\\|__|__|xx|__|
//        12  |__|xx|__|//|__|__|__|__|__|__|__|__|__|\\|__|xx|__|
//        13  |__|xx|__|//|__|__|__|__|__|__|__|__|__|\\|__|xx|__|
//        14  |__|xx|//|__|__|__|__|__|__|__|__|__|__|__|\\|xx|__|
//        15  |__|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|xx|__|
//        16  |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//           (0,-1)                                          (1,-1)
//
// The capital letter A is draw from (2,14)--(8,2), (8,2)--(14,14), and
// (4,10)--(12,10) so it's index list is [2,14,8,2,8,2,14,14,4,10,12,10]
// and will be drawn in the bottom right quadrant of the screen in model
// space.
//
//================================================================================

#ifndef CHARMESH
#define CHARMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Application.hpp"
#include <unordered_map>
#include <vector>

namespace mare
{

class CharMesh : public SimpleMesh<float>
{
public:
    CharMesh(std::string str, float keying = 1.0f) //, bool dynamic, size_t size_in_bytes)
    {
        // Set defaults
        this->keying = keying;
        this->dynamic = dynamic;
        this->size_in_bytes = size_in_bytes;
        this->str = str;
        this->color = glm::vec4(1.0f);
        std::vector<float> verts = string_to_verts(str);
        render_state->set_draw_method(DrawMethod::LINES);

        // if dynamic, create extra space to hold resized strings
        vertex_buffers = Application::GenBuffer<float>(1);
        vertex_buffers->create(verts);
        vertex_buffers->set_format({{ShaderDataType::VEC2, "position"}});

        render_state->create();
        render_state->add_vertex_buffer(vertex_buffers);
    }
    glm::vec2 get_center() const
    {
        glm::vec2 label_center = {max_width / 2.0f, lines / 2.0f};
        glm::vec3 label_scale = get_scale();
        glm::vec2 true_label_center = {get_position().x + 2.0f * label_scale.x * label_center.x, get_position().y - 2.0f * label_scale.y * label_center.y};
        return true_label_center;
    }
    // must set the scale first to know where to center
    void set_center(glm::vec3 center)
    {
        glm::vec2 label_center = {max_width / 2.0f, lines / 2.0f};
        glm::vec3 label_scale = get_scale();
        glm::vec2 label_top_left = glm::vec2(center.x - label_scale.x * label_center.x, center.y + label_scale.y * (label_center.y + 1.0f));
        set_position(glm::vec3(label_top_left, 0.0f));
    }

private:
    int lines = 1;
    int max_width = 0;
    float keying = 1.0f;
    bool dynamic = false;
    size_t size_in_bytes;
    std::string str;
    glm::vec4 color;
    float grid_points[17] = {0.0f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f, 0.5625f, 0.625f, 0.6875f, 0.75f, 0.8125f, 0.875f, 0.9375f, 1.0f};
    std::unordered_map<char, std::vector<unsigned int>> ASCII_font = {
        {'!', std::vector<unsigned int>{8, 2, 8, 11, 8, 13, 8, 14}},
        {'"', std::vector<unsigned int>{5, 2, 5, 4, 11, 2, 11, 4}},
        {'#', std::vector<unsigned int>{7, 2, 3, 14, 13, 2, 9, 14, 2, 5, 14, 5, 2, 11, 14, 11}},
        {'$', std::vector<unsigned int>{2, 12, 14, 12, 14, 12, 14, 8, 14, 8, 2, 8, 2, 8, 2, 4, 2, 4, 14, 4, 8, 2, 8, 14}},
        {'%', std::vector<unsigned int>{2, 2, 2, 6, 2, 6, 6, 6, 6, 6, 6, 2, 6, 2, 2, 2, 2, 14, 14, 2, 10, 10, 10, 14, 10, 14, 14, 14, 14, 14, 14, 10, 14, 10, 10, 10}},
        {'&', std::vector<unsigned int>{12, 14, 6, 6, 6, 6, 6, 4, 6, 4, 7, 2, 7, 2, 9, 2, 9, 2, 10, 4, 10, 4, 10, 6, 10, 6, 4, 10, 4, 10, 4, 13, 4, 13, 6, 14, 6, 14, 10, 14, 10, 14, 12, 11}},
        {'\'', std::vector<unsigned int>{8, 2, 8, 4}},
        {'(', std::vector<unsigned int>{8, 2, 6, 6, 6, 6, 6, 10, 6, 10, 8, 14}},
        {')', std::vector<unsigned int>{8, 2, 10, 6, 10, 6, 10, 10, 10, 10, 8, 14}},
        {'*', std::vector<unsigned int>{8, 5, 8, 11, 6, 6, 10, 10, 6, 10, 10, 6}},
        {'+', std::vector<unsigned int>{8, 5, 8, 11, 5, 8, 11, 8}},
        {',', std::vector<unsigned int>{7, 14, 9, 14, 9, 14, 9, 12}},
        {'-', std::vector<unsigned int>{5, 8, 11, 8}},
        {'.', std::vector<unsigned int>{8, 13, 8, 14}},
        {'/', std::vector<unsigned int>{11, 2, 5, 14}},
        {'0', std::vector<unsigned int>{2, 2, 14, 2, 14, 2, 14, 14, 14, 14, 2, 14, 2, 14, 2, 2, 2, 14, 14, 2}},
        {'1', std::vector<unsigned int>{6, 4, 8, 2, 8, 2, 8, 14, 6, 14, 10, 14}},
        {'2', std::vector<unsigned int>{2, 4, 2, 2, 2, 2, 14, 2, 14, 2, 14, 8, 14, 8, 2, 8, 2, 8, 2, 14, 2, 14, 14, 14}},
        {'3', std::vector<unsigned int>{2, 2, 14, 2, 2, 8, 14, 8, 2, 14, 14, 14, 14, 2, 14, 14}},
        {'4', std::vector<unsigned int>{2, 2, 2, 8, 14, 2, 14, 14, 2, 8, 14, 8}},
        {'5', std::vector<unsigned int>{14, 2, 2, 2, 2, 2, 2, 8, 2, 8, 14, 8, 14, 8, 14, 14, 14, 14, 2, 14, 2, 14, 2, 12}},
        {'6', std::vector<unsigned int>{14, 4, 14, 2, 14, 2, 2, 2, 2, 2, 2, 14, 2, 14, 14, 14, 14, 14, 14, 8, 14, 8, 2, 8}},
        {'7', std::vector<unsigned int>{2, 2, 14, 2, 14, 2, 2, 14}},
        {'8', std::vector<unsigned int>{2, 2, 14, 2, 14, 2, 14, 14, 14, 14, 2, 14, 2, 14, 2, 2, 2, 8, 14, 8}},
        {'9', std::vector<unsigned int>{14, 8, 2, 8, 2, 8, 2, 2, 2, 2, 14, 2, 14, 2, 14, 14, 14, 14, 2, 14, 2, 14, 2, 12}},
        {':', std::vector<unsigned int>{8, 3, 8, 5, 8, 13, 8, 11}},
        {';', std::vector<unsigned int>{8, 3, 8, 5, 7, 14, 9, 14, 9, 14, 9, 12}},
        {'<', std::vector<unsigned int>{14, 2, 2, 8, 2, 8, 14, 14}},
        {'=', std::vector<unsigned int>{2, 5, 14, 5, 2, 11, 14, 11}},
        {'>', std::vector<unsigned int>{2, 2, 14, 8, 14, 8, 2, 14}},
        {'?', std::vector<unsigned int>{2, 4, 2, 2, 2, 2, 14, 2, 14, 2, 14, 8, 14, 8, 8, 8, 8, 8, 8, 11, 8, 13, 8, 14}},
        {'@', std::vector<unsigned int>{10, 10, 6, 10, 6, 10, 6, 6, 6, 6, 10, 6, 10, 6, 10, 12, 10, 12, 12, 12, 12, 12, 14, 10, 14, 10, 14, 6, 14, 6, 10, 2, 10, 2, 6, 2, 6, 2, 2, 6, 2, 6, 2, 10, 2, 10, 6, 14, 6, 14, 10, 14}},
        {'A', std::vector<unsigned int>{2, 14, 8, 2, 8, 2, 14, 14, 4, 10, 12, 10}},
        {'B', std::vector<unsigned int>{2, 2, 2, 14, 2, 2, 11, 2, 11, 2, 11, 7, 2, 7, 14, 7, 2, 14, 14, 14, 14, 14, 14, 7}},
        {'C', std::vector<unsigned int>{14, 4, 14, 2, 14, 2, 2, 2, 2, 2, 2, 14, 2, 14, 14, 14, 14, 14, 14, 12}},
        {'D', std::vector<unsigned int>{2, 2, 8, 2, 8, 2, 14, 5, 14, 5, 14, 11, 14, 11, 8, 14, 8, 14, 2, 14, 2, 14, 2, 2}},
        {'E', std::vector<unsigned int>{2, 14, 2, 2, 2, 2, 14, 2, 2, 8, 8, 8, 2, 14, 14, 14}},
        {'F', std::vector<unsigned int>{2, 14, 2, 2, 2, 2, 14, 2, 2, 8, 8, 8}},
        {'G', std::vector<unsigned int>{12, 4, 12, 2, 12, 2, 2, 2, 2, 2, 2, 14, 2, 14, 12, 14, 12, 14, 12, 8, 14, 8, 8, 8}},
        {'H', std::vector<unsigned int>{2, 2, 2, 14, 14, 2, 14, 14, 2, 8, 14, 8}},
        {'I', std::vector<unsigned int>{2, 2, 14, 2, 2, 14, 14, 14, 8, 2, 8, 14}},
        {'J', std::vector<unsigned int>{2, 2, 14, 2, 11, 2, 11, 11, 11, 11, 8, 14, 8, 14, 5, 14, 5, 14, 2, 11}},
        {'K', std::vector<unsigned int>{2, 2, 2, 14, 2, 8, 14, 2, 2, 8, 14, 14}},
        {'L', std::vector<unsigned int>{2, 2, 2, 14, 2, 14, 14, 14}},
        {'M', std::vector<unsigned int>{2, 14, 2, 2, 2, 2, 8, 11, 8, 11, 14, 2, 14, 2, 14, 14}},
        {'N', std::vector<unsigned int>{2, 14, 2, 2, 2, 2, 14, 14, 14, 14, 14, 2}},
        {'O', std::vector<unsigned int>{2, 2, 14, 2, 14, 2, 14, 14, 14, 14, 2, 14, 2, 14, 2, 2}},
        {'P', std::vector<unsigned int>{2, 14, 2, 2, 2, 2, 14, 2, 14, 2, 14, 8, 14, 8, 2, 8}},
        {'Q', std::vector<unsigned int>{2, 2, 14, 2, 14, 2, 14, 14, 14, 14, 2, 14, 2, 14, 2, 2, 11, 11, 16, 16}},
        {'R', std::vector<unsigned int>{2, 14, 2, 2, 2, 2, 14, 2, 14, 2, 14, 8, 14, 8, 2, 8, 8, 8, 14, 14}},
        {'S', std::vector<unsigned int>{14, 5, 14, 2, 14, 2, 2, 2, 2, 2, 2, 8, 2, 8, 14, 8, 14, 8, 14, 14, 14, 14, 2, 14, 2, 14, 2, 11}},
        {'T', std::vector<unsigned int>{2, 2, 14, 2, 8, 2, 8, 14}},
        {'U', std::vector<unsigned int>{2, 2, 2, 14, 2, 14, 14, 14, 14, 14, 14, 2}},
        {'V', std::vector<unsigned int>{2, 2, 8, 14, 8, 14, 14, 2}},
        {'W', std::vector<unsigned int>{2, 2, 2, 14, 2, 14, 8, 5, 8, 5, 14, 14, 14, 14, 14, 2}},
        {'X', std::vector<unsigned int>{2, 2, 14, 14, 14, 2, 2, 14}},
        {'Y', std::vector<unsigned int>{2, 2, 8, 8, 8, 8, 14, 2, 8, 8, 8, 14}},
        {'Z', std::vector<unsigned int>{2, 2, 14, 2, 14, 2, 2, 14, 2, 14, 14, 14}},
        {'[', std::vector<unsigned int>{8, 0, 2, 0, 2, 0, 2, 16, 2, 16, 8, 16}},
        {'\\', std::vector<unsigned int>{5, 2, 11, 14}},
        {']', std::vector<unsigned int>{8, 0, 14, 0, 14, 0, 14, 16, 14, 16, 8, 16}},
        {'^', std::vector<unsigned int>{5, 5, 8, 2, 8, 2, 11, 5}},
        {'_', std::vector<unsigned int>{0, 16, 16, 16}},
        {'`', std::vector<unsigned int>{7, 2, 9, 5}},
        {'a', std::vector<unsigned int>{2, 10, 2, 14, 2, 14, 14, 14, 14, 14, 14, 8, 14, 10, 2, 10, 14, 8, 2, 8}},
        {'b', std::vector<unsigned int>{2, 2, 2, 14, 2, 14, 14, 14, 14, 14, 14, 8, 14, 8, 2, 8}},
        {'c', std::vector<unsigned int>{2, 8, 2, 14, 2, 14, 14, 14, 14, 8, 2, 8}},
        {'d', std::vector<unsigned int>{2, 8, 2, 14, 2, 14, 14, 14, 14, 14, 14, 2, 14, 8, 2, 8}},
        {'e', std::vector<unsigned int>{2, 8, 2, 14, 2, 14, 14, 14, 14, 11, 14, 8, 14, 8, 2, 8, 2, 11, 14, 11}},
        {'f', std::vector<unsigned int>{8, 14, 8, 2, 8, 2, 14, 2, 14, 2, 14, 5, 2, 8, 14, 8}},
        {'g', std::vector<unsigned int>{2, 8, 2, 14, 2, 14, 14, 14, 14, 16, 14, 8, 14, 8, 2, 8, 14, 16, 2, 16}},
        {'h', std::vector<unsigned int>{2, 14, 2, 2, 2, 8, 14, 8, 14, 8, 14, 14}},
        {'i', std::vector<unsigned int>{8, 5, 8, 14, 8, 2, 8, 3}},
        {'j', std::vector<unsigned int>{8, 5, 8, 16, 8, 16, 2, 16, 8, 2, 8, 3}},
        {'k', std::vector<unsigned int>{2, 2, 2, 14, 2, 11, 14, 8, 2, 11, 14, 14}},
        {'l', std::vector<unsigned int>{8, 2, 8, 14}},
        {'m', std::vector<unsigned int>{2, 8, 2, 14, 8, 8, 8, 14, 14, 8, 14, 14, 2, 8, 14, 8}},
        {'n', std::vector<unsigned int>{2, 8, 2, 14, 14, 8, 14, 14, 2, 8, 14, 8}},
        {'o', std::vector<unsigned int>{2, 8, 2, 14, 14, 8, 14, 14, 2, 8, 14, 8, 2, 14, 14, 14}},
        {'p', std::vector<unsigned int>{2, 8, 2, 16, 14, 8, 14, 14, 2, 8, 14, 8, 2, 14, 14, 14}},
        {'q', std::vector<unsigned int>{2, 8, 2, 14, 14, 8, 14, 16, 2, 8, 14, 8, 2, 14, 14, 14}},
        {'r', std::vector<unsigned int>{2, 8, 2, 14, 14, 8, 14, 11, 2, 8, 14, 8}},
        {'s', std::vector<unsigned int>{2, 8, 2, 11, 14, 11, 14, 14, 2, 8, 14, 8, 2, 14, 14, 14, 2, 11, 14, 11}},
        {'t', std::vector<unsigned int>{8, 2, 8, 14, 2, 5, 14, 5}},
        {'u', std::vector<unsigned int>{2, 8, 2, 14, 14, 8, 14, 14, 2, 14, 14, 14}},
        {'v', std::vector<unsigned int>{2, 8, 8, 14, 8, 14, 14, 8}},
        {'w', std::vector<unsigned int>{2, 8, 5, 14, 5, 14, 8, 8, 8, 8, 11, 14, 11, 14, 14, 8}},
        {'x', std::vector<unsigned int>{2, 8, 14, 14, 14, 8, 2, 14}},
        {'y', std::vector<unsigned int>{2, 8, 8, 14, 6, 16, 14, 8}},
        {'z', std::vector<unsigned int>{2, 8, 14, 8, 14, 8, 2, 14, 2, 14, 14, 14}},
        {'{', std::vector<unsigned int>{10, 2, 8, 2, 8, 2, 7, 4, 7, 4, 7, 7, 7, 7, 5, 8, 5, 8, 7, 9, 7, 9, 7, 12, 7, 12, 8, 14, 8, 14, 10, 14}},
        {'|', std::vector<unsigned int>{8, 0, 8, 16}},
        {'}', std::vector<unsigned int>{6, 2, 8, 2, 8, 2, 9, 4, 9, 4, 9, 7, 9, 7, 11, 8, 11, 8, 9, 9, 9, 9, 9, 12, 9, 12, 8, 14, 8, 14, 6, 14}},
        {'~', std::vector<unsigned int>{2, 10, 2, 8, 2, 8, 4, 6, 4, 6, 6, 6, 6, 6, 10, 10, 10, 10, 12, 10, 12, 10, 14, 8, 14, 8, 14, 6}}};

    // Get X, Y points from letter grid indices
    std::vector<float> indices_to_points(std::vector<unsigned int> indices)
    {
        size_t n = indices.size();
        std::vector<float> points;
        for (size_t i = 0; i < n; i++)
        {
            if (i % 2 == 0)
            {
                points.push_back(grid_points[indices[i]]);
            }
            else
            {
                points.push_back(-grid_points[indices[i]]);
            }
        }
        return points;
    }
    // Return a letter made of X, Y points on the letter grid from an ASCII character
    std::vector<float> char_to_lines(char letter)
    {
        return indices_to_points(ASCII_font[letter]);
    }
    // Move letter right by keying amount
    std::vector<float> move_right(std::vector<float> letter, unsigned int spaces)
    {
        for (int i = 0; i < letter.size() / 2; i++)
        {
            letter[2 * i] += float(spaces);
        }
        return letter;
    }
    //Move letter down by text height
    std::vector<float> move_down(std::vector<float> letter, unsigned int spaces)
    {
        for (int i = 0; i < letter.size() / 2; i++)
        {
            letter[2 * i + 1] -= float(spaces);
        }
        return letter;
    }

    // Create letter List
    std::vector<std::vector<float>> create_letter_list(std::string str)
    {
        // Create list of letters for the vertex array
        std::vector<std::vector<float>> letter_list;
        int line_width = 0;
        int caret_pos = 0;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == '\n')
            {
                lines += 1;
                caret_pos = -1;
                line_width = 0;
            }

            letter_list.push_back(move_down(move_right(char_to_lines(str[i]), unsigned int(float(caret_pos) * keying)), lines));
            caret_pos += 1;
            line_width += 1;
            if (line_width >= max_width)
            {
                max_width = line_width;
            }
        }
        return letter_list;
    }

    std::vector<float> string_to_verts(std::string str)
    {
        std::vector<std::vector<float>> letter_list = create_letter_list(str);
        std::vector<float> vertices;
        for (int i = 0; i < letter_list.size(); i++)
        {
            for (int j = 0; j < letter_list[i].size(); j++)
            {
                vertices.push_back(letter_list[i][j]);
            }
        }
        return vertices;
    }
};
} // namespace mare
#endif