#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
Asset::~Asset() {}
Component::~Component() {}
System::~System() {}
namespace math
{
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
} // namespace math

} // namespace mare