#ifndef MARE
#define MARE

// External Libraries
#include "glm.hpp"
// Standard Library
#include <vector>
#include <variant>
#include <memory>

namespace mare
{
// wrap in until namespace
// Create Asset class for all user created assets
class Asset
{
public:
    virtual ~Asset() = 0;
};
// for all user created systems and components
class Component : public Asset
{
public:
    virtual ~Component() = 0;
};

// Scopes and References
template <typename T>
using Scoped = std::unique_ptr<T>;
template <typename T>
using Referenced = std::shared_ptr<T>;
// Variant type for scalars, vectors, and matrices
using shader_data_type = std::variant<unsigned int, int, unsigned short, short, unsigned char, float, bool, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4, glm::mat2, glm::mat3, glm::mat4>;

// Create scoped or referenced types
template <typename T, typename... Args>
Scoped<T> gen_scoped(Args... args)
{
    return std::make_unique<T>(args...);
}
template <typename T, typename... Args>
Referenced<T> gen_ref(Args... args)
{
    return std::make_shared<T>(args...);
}

namespace util
{
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
} // namespace util

namespace math
{
// Math algorithms
float shoelace(std::vector<glm::vec2> points);
// Coordinate conversions
void normalized_device_coordinates(int x, int y, float &nx, float &ny);
void window_coordinates(float nx, float ny, int &x, int &y);

// Math constants
const float PI = 3.141592653f;
} // namespace math

} // namespace mare

#endif