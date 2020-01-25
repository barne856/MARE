#include "mare/MareUtility.hpp"

namespace mare
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
} // namespace mare