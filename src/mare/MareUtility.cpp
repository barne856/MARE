#include "mare/MareUtility.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
Asset::~Asset() {}
Component::~Component() {}
size_t shader_data_type_size(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::FLOAT:
        return 4;
    case ShaderDataType::VEC2:
        return 4 * 2;
    case ShaderDataType::VEC3:
        return 4 * 3;
    case ShaderDataType::VEC4:
        return 4 * 4;
    case ShaderDataType::MAT2:
        return 4 * 2 * 2;
    case ShaderDataType::MAT3:
        return 4 * 3 * 3;
    case ShaderDataType::MAT4:
        return 4 * 4 * 4;
    case ShaderDataType::INT:
        return 4;
    case ShaderDataType::INT2:
        return 4 * 2;
    case ShaderDataType::INT3:
        return 4 * 3;
    case ShaderDataType::INT4:
        return 4 * 4;
    case ShaderDataType::BOOL:
        return 1;
    case ShaderDataType::BYTE:
        return 1;
    case ShaderDataType::UNSIGNED_SHORT:
        return 2;
    case ShaderDataType::SHORT:
        return 2;
    case ShaderDataType::UNSIGNED_INT:
        return 4;
    default:
        return 0;
    }
    return 0;
}
size_t shader_component_count(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::FLOAT:
        return 1;
    case ShaderDataType::VEC2:
        return 2;
    case ShaderDataType::VEC3:
        return 3;
    case ShaderDataType::VEC4:
        return 4;
    case ShaderDataType::MAT2:
        return 2 * 2;
    case ShaderDataType::MAT3:
        return 3 * 3;
    case ShaderDataType::MAT4:
        return 4 * 4;
    case ShaderDataType::INT:
        return 1;
    case ShaderDataType::INT2:
        return 2;
    case ShaderDataType::INT3:
        return 3;
    case ShaderDataType::INT4:
        return 4;
    case ShaderDataType::BOOL:
        return 1;
    case ShaderDataType::BYTE:
        return 1;
    case ShaderDataType::UNSIGNED_SHORT:
        return 1;
    case ShaderDataType::SHORT:
        return 1;
    case ShaderDataType::UNSIGNED_INT:
        return 1;
    default:
        return 0;
    }
    return 0;
}

float shoelace(std::vector<glm::vec2> points)
{
    float A{};
    for (size_t i = 0; i < points.size(); i++)
    {
        size_t j = i + 1;
        if (j >= points.size())
        {
            j = 0;
        }
        A += points[i].x * points[j].y;
    }
    for (size_t i = 0; i < points.size(); i++)
    {
        size_t j = i + 1;
        if (j >= points.size())
        {
            j = 0;
        }
        A -= points[i].y * points[j].x;
    }
    return A;
}

// get normalized device coordinates from window coordinates
void normalized_device_coordinates(int x, int y, float &nx, float &ny)
{
    nx = float(2 * x) / float(Renderer::get_info().window_width) - 1.0f;
    ny = 1.0f - float(2 * y) / float(Renderer::get_info().window_height);
}

// get window coordinates from normalized device coordinates
void window_coordinates(float nx, float ny, int &x, int &y)
{
    x = static_cast<int>(float(Renderer::get_info().window_width) * (nx + 1.0f) / 2.0f);
    y = static_cast<int>(float(Renderer::get_info().window_height) * (ny - 1.0f) / -2.0f);
}

} // namespace mare