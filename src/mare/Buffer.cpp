#include "mare/Buffer.hpp"

namespace mare
{
template <typename T>
ShaderDataType glm_to_shader_type()
{
    return ShaderDataType::NONE;
}
template <>
ShaderDataType glm_to_shader_type<float>()
{
    return ShaderDataType::FLOAT;
}
template <>
ShaderDataType glm_to_shader_type<glm::vec2>()
{
    return ShaderDataType::VEC2;
}
template <>
ShaderDataType glm_to_shader_type<glm::vec3>()
{
    return ShaderDataType::VEC3;
}
template <>
ShaderDataType glm_to_shader_type<glm::vec4>()
{
    return ShaderDataType::VEC4;
}
template <>
ShaderDataType glm_to_shader_type<glm::mat2>()
{
    return ShaderDataType::MAT2;
}
template <>
ShaderDataType glm_to_shader_type<glm::mat3>()
{
    return ShaderDataType::MAT3;
}
template <>
ShaderDataType glm_to_shader_type<glm::mat4>()
{
    return ShaderDataType::MAT4;
}
template <>
ShaderDataType glm_to_shader_type<int>()
{
    return ShaderDataType::INT;
}
template <>
ShaderDataType glm_to_shader_type<glm::ivec2>()
{
    return ShaderDataType::INT2;
}
template <>
ShaderDataType glm_to_shader_type<glm::ivec3>()
{
    return ShaderDataType::INT3;
}
template <>
ShaderDataType glm_to_shader_type<glm::ivec4>()
{
    return ShaderDataType::INT4;
}
template <>
ShaderDataType glm_to_shader_type<bool>()
{
    return ShaderDataType::BOOL;
}
template <>
ShaderDataType glm_to_shader_type<char>()
{
    return ShaderDataType::CHAR;
}
template <>
ShaderDataType glm_to_shader_type<unsigned char>()
{
    return ShaderDataType::BYTE;
}
template <>
ShaderDataType glm_to_shader_type<short>()
{
    return ShaderDataType::SHORT;
}
template <>
ShaderDataType glm_to_shader_type<unsigned short>()
{
    return ShaderDataType::UNSIGNED_SHORT;
}
template <>
ShaderDataType glm_to_shader_type<unsigned int>()
{
    return ShaderDataType::UNSIGNED_INT;
}
} // namespace mare