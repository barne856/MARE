#ifndef MAREUTILITY
#define MAREUTILITY

#include "glm.hpp"
#include <vector>
namespace mare
{
// Math algorithms
float shoelace(std::vector<glm::vec2> points);
// Coordinate conversions
void normalized_device_coordinates(int x, int y, float &nx, float &ny);
void window_coordinates(float nx, float ny, int &x, int &y);
} // namespace mare

#endif