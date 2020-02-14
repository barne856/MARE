#ifndef MAREUTILITY
#define MAREUTILITY

// External Libraries
#include "glm.hpp"
// Standard Library
#include <vector>
#include <variant>

namespace mare
{
// Variant type for scalars, vectors, and matrices
using linalg_type = std::variant<unsigned int, int, unsigned short, short, unsigned char, float, bool, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4, glm::mat2, glm::mat3, glm::mat4>;
enum class LinalgDataType
{
    UNSIGNED_INT = 0,
    INT,
    UNSIGNED_SHORT,
    SHORT,
    BYTE,
    FLOAT,
    BOOL,
    VEC2,
    VEC3,
    VEC4,
    INT2,
    INT3,
    INT4,
    MAT2,
    MAT3,
    MAT4
};

size_t linalg_data_type_size(LinalgDataType type);
size_t linalg_component_count(LinalgDataType type);

// template <class... Ts>
// struct overload : Ts...
// {
//     using Ts::operator()...;
// };
// template <class... Ts>
// overload(Ts...)->overload<Ts...>;
// size_t linalg_current_size(linalg_type v)
// {
//     std::visit(overload{
//                    [](unsigned int &) { return sizeof(unsigned int); },
//                    [](int &) { return sizeof(int); },
//                    [](unsigned short &) { return sizeof(unsigned short); },
//                    [](short &) { return sizeof(short); },
//                    [](unsigned char &) { return sizeof(unsigned char); },
//                    [](float &) { return sizeof(float); },
//                    [](bool &) { return sizeof(bool); },
//                    [](glm::vec2 &) { return sizeof(glm::vec2); },
//                    [](glm::vec3 &) { return sizeof(glm::vec3); },
//                    [](glm::vec4 &) { return sizeof(glm::vec4); },
//                    [](glm::ivec2 &) { return sizeof(glm::ivec2); },
//                    [](glm::ivec3 &) { return sizeof(glm::ivec3); },
//                    [](glm::ivec4 &) { return sizeof(glm::ivec4); },
//                    [](glm::mat2 &) { return sizeof(glm::mat2); },
//                    [](glm::mat3 &) { return sizeof(glm::mat3); },
//                    [](glm::mat4 &) { return sizeof(glm::mat4); }},
//                v);
//     return 0;
// }

// get index of a type in a variant - "get_index<type, variant_type>"
template <typename>
struct tag
{
};
template <typename T, typename V>
struct get_index;
template <typename T, typename... Ts>
struct get_index<T, std::variant<Ts...>>
    : std::integral_constant<size_t, std::variant<tag<Ts>...>(tag<T>()).index()>
{
};

// Math algorithms
float shoelace(std::vector<glm::vec2> points);
// Coordinate conversions
void normalized_device_coordinates(int x, int y, float &nx, float &ny);
void window_coordinates(float nx, float ny, int &x, int &y);

} // namespace mare

#endif