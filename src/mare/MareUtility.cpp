#include "mare/MareUtility.hpp"
#include "mare/Renderer.hpp"

namespace mare
{

size_t linalg_data_type_size(LinalgDataType type)
{
    switch (type)
    {
    case LinalgDataType::FLOAT:
        return 4;
    case LinalgDataType::VEC2:
        return 4 * 2;
    case LinalgDataType::VEC3:
        return 4 * 3;
    case LinalgDataType::VEC4:
        return 4 * 4;
    case LinalgDataType::MAT2:
        return 4 * 2 * 2;
    case LinalgDataType::MAT3:
        return 4 * 3 * 3;
    case LinalgDataType::MAT4:
        return 4 * 4 * 4;
    case LinalgDataType::INT:
        return 4;
    case LinalgDataType::INT2:
        return 4 * 2;
    case LinalgDataType::INT3:
        return 4 * 3;
    case LinalgDataType::INT4:
        return 4 * 4;
    case LinalgDataType::BOOL:
        return 1;
    case LinalgDataType::BYTE:
        return 1;
    case LinalgDataType::UNSIGNED_SHORT:
        return 2;
    case LinalgDataType::SHORT:
        return 2;
    case LinalgDataType::UNSIGNED_INT:
        return 4;
    default:
        return 0;
    }
    return 0;
}
size_t linalg_component_count(LinalgDataType type)
{
    switch (type)
    {
    case LinalgDataType::FLOAT:
        return 1;
    case LinalgDataType::VEC2:
        return 2;
    case LinalgDataType::VEC3:
        return 3;
    case LinalgDataType::VEC4:
        return 4;
    case LinalgDataType::MAT2:
        return 2 * 2;
    case LinalgDataType::MAT3:
        return 3 * 3;
    case LinalgDataType::MAT4:
        return 4 * 4;
    case LinalgDataType::INT:
        return 1;
    case LinalgDataType::INT2:
        return 2;
    case LinalgDataType::INT3:
        return 3;
    case LinalgDataType::INT4:
        return 4;
    case LinalgDataType::BOOL:
        return 1;
    case LinalgDataType::BYTE:
        return 1;
    case LinalgDataType::UNSIGNED_SHORT:
        return 1;
    case LinalgDataType::SHORT:
        return 1;
    case LinalgDataType::UNSIGNED_INT:
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