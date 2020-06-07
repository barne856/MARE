// MARE
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"

// Standard Library
#include <cmath>
#include <limits>
#include <math.h>
#include <sstream>
#include <string>

namespace mare {
System::~System() {}
namespace math {
float angle_between(glm::vec3 v1, glm::vec3 v2) {
  return acosf(glm::dot(v1, v2) / glm::length(v1) / glm::length(v2));
}
float shoelace(std::vector<glm::vec2> points) {
  float A{};
  for (size_t i = 0; i < points.size(); i++) {
    size_t j = i + 1;
    if (j >= points.size()) {
      j = 0;
    }
    A += points[i].x * points[j].y;
  }
  for (size_t i = 0; i < points.size(); i++) {
    size_t j = i + 1;
    if (j >= points.size()) {
      j = 0;
    }
    A -= points[i].y * points[j].x;
  }
  return A;
}

void normalized_device_coordinates(int x, int y, float &nx, float &ny) {
  nx = float(2 * x) / float(Renderer::get_info().window_width) - 1.0f;
  ny = 1.0f - float(2 * y) / float(Renderer::get_info().window_height);
}

void window_coordinates(float nx, float ny, int &x, int &y) {
  x = static_cast<int>(float(Renderer::get_info().window_width) * (nx + 1.0f) /
                       2.0f);
  y = static_cast<int>(float(Renderer::get_info().window_height) * (ny - 1.0f) /
                       -2.0f);
}

bool is_in_polygon(glm::vec2 point, std::vector<glm::vec2> polygon) {
  float polygon_area = math::shoelace(polygon);
  polygon.push_back(point);
  for (int i = polygon.size() - 1; i > 0; i--) {
    if (polygon_area < math::shoelace(polygon)) {
      return false;
    }
    std::iter_swap(polygon.begin() + i, polygon.begin() + i - 1);
  }
  return true;
}
glm::vec2 snap_to_line(glm::vec2 point, glm::vec2 p1, glm::vec2 p2) {
  // find closest point on line of infinite length
  float lambda = glm::dot(point - p1, p2 - p1) / glm::dot(p2 - p1, p2 - p1);
  glm::vec2 p3;
  if (lambda <= 0.0f) {
    return p1;
  } else if (lambda >= 1.0f) {
    return p2;
  } else {
    return p1 + lambda * (p2 - p1);
  }
}
glm::vec2 intersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
  if (p1 == p2 || p3 == p4) {
    return glm::vec2(std::nanf(""));
  }
  // sort line 1 by increasing x
  float m1 = std::numeric_limits<float>::infinity();
  float b1 = std::numeric_limits<float>::infinity();
  bool line1_vert = false;
  if (p1.x > p2.x) {
    std::swap(p1, p2);
  } else if (p1.x == p2.x) {
    line1_vert = true;
  }
  if (!line1_vert) {
    m1 = (p2.y - p1.y) / (p2.x - p1.x);
    b1 = p1.y - p1.x * m1;
  }
  // sort line 2 by increasing x
  float m2 = std::numeric_limits<float>::infinity();
  float b2 = std::numeric_limits<float>::infinity();
  bool line2_vert = false;
  if (p3.x > p4.x) {
    std::swap(p3, p4);
  } else if (p3.x == p4.x) {
    line2_vert = true;
  }
  if (!line2_vert) {
    m2 = (p4.y - p3.y) / (p4.x - p3.x);
    b2 = p3.y - p3.x * m2;
  }
  // check if lines are parallel
  if (m1 == m2) {
    return glm::vec2(std::nanf(""));
  }
  // calculate intersection point
  if (line1_vert) {
    return glm::vec2(p1.x, m2 * p1.x + b2);
  } else if (line2_vert) {
    return glm::vec2(p3.x, m1 * p3.x + b1);
  } else {
    float x = (b1 - b2) / (m2 - m1);
    float y = m1 * x + b1;
    return glm::vec2(x, y);
  }
}
glm::vec2 clamp_point_to_triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3,
                                  glm::vec2 point) {
  // vertices should be provided in counter-clockwise order.
  assert(shoelace({v1, v2, v3}) >= 0.0f);
  // if point is inside of polygon, return point.
  if (is_in_polygon(point, {v1, v2, v3})) {
    return point;
  }
  // else the closest point is on an edge of the triangle.
  glm::vec2 p1 = math::snap_to_line(point, v2, v1);
  glm::vec2 p2 = math::snap_to_line(point, v3, v2);
  glm::vec2 p3 = math::snap_to_line(point, v1, v3);
  float l1 = length(point - p1);
  float l2 = length(point - p2);
  float l3 = length(point - p3);
  if (l1 <= l2 && l1 <= l3) {
    return p1;
  } else if (l2 <= l1 && l2 <= l3) {
    return p2;
  } else {
    return p3;
  }
}
} // namespace math

namespace util {

glm::vec4 rgba_to_hsva(glm::vec4 color) {
  float cmax = std::max(std::max(color.r, color.g), color.b);
  float cmin = std::min(std::min(color.r, color.g), color.b);
  float delta = cmax - cmin;
  glm::vec4 hsva_color{};
  // H
  if (delta == 0.0f) {
    hsva_color[0] = 0.0f;
  } else if (cmax == color.r) {
    hsva_color[0] = fmod((math::PI / 3.0f) * ((color.g - color.b) / delta),
                         2.0f * math::PI);
  } else if (cmax == color.g) {
    hsva_color[0] =
        fmod((math::PI / 3.0f) * ((color.b - color.r) / delta + 2.0f),
             2.0f * math::PI);
  } else {
    hsva_color[0] =
        fmod((math::PI / 3.0f) * ((color.r - color.g) / delta + 4.0f),
             2.0f * math::PI);
  }
  // S
  if (cmax == 0) {
    hsva_color[1] = 0.0f;
  } else {
    hsva_color[1] = delta / cmax;
  }
  // V
  hsva_color[2] = cmax;
  // A
  hsva_color[3] = color.a;
  return hsva_color;
}

glm::vec4 hsva_to_rgba(glm::vec4 color) {
  float h = color[0] - math::TAU * floor(color[0] / math::TAU);
  float C = color[1] * color[2];
  float X = C * (1.0f - fabs(fmod(3.0f * h / math::PI, 2.0f) - 1.0f));
  float m = color[2] - C;
  glm::vec4 rgba_color;
  if (0.0f <= h && h < math::PI / 3.0f) {
    rgba_color = glm::vec4(C, X, 0.0f, color.a);
  } else if (math::PI / 3.0f <= h && h < 2.0f * math::PI / 3.0f) {
    rgba_color = glm::vec4(X, C, 0.0f, color.a);
  } else if (2.0f * math::PI / 3.0f <= h && h < math::PI) {
    rgba_color = glm::vec4(0.0f, C, X, color.a);
  } else if (math::PI <= h && h < 4.0f * math::PI / 3.0f) {
    rgba_color = glm::vec4(0.0f, X, C, color.a);
  } else if (4.0f * math::PI / 3.0f <= h && h < 5.0f * math::PI / 3.0f) {
    rgba_color = glm::vec4(X, 0.0f, C, color.a);
  } else if (5.0f * math::PI / 3.0f <= h && h < 2.0f * math::PI) {
    rgba_color = glm::vec4(C, 0.0f, X, color.a);
  }
  rgba_color += m;
  rgba_color.a = color.a;
  return rgba_color;
}
std::string rgb_to_hex(glm::vec3 color, bool with_head) {
  std::stringstream ss;
  int r, g, b;
  if (color.r < 0.5f) {
    r = glm::clamp(static_cast<int>(floor(color.r * 255.0f)), 0, 255);
  } else {
    r = glm::clamp(static_cast<int>(ceil(color.r * 255.0f)), 0, 255);
  }
  if (color.g < 0.5f) {
    g = glm::clamp(static_cast<int>(floor(color.g * 255.0f)), 0, 255);
  } else {
    g = glm::clamp(static_cast<int>(ceil(color.g * 255.0f)), 0, 255);
  }
  if (color.b < 0.5f) {
    b = glm::clamp(static_cast<int>(floor(color.b * 255.0f)), 0, 255);
  } else {
    b = glm::clamp(static_cast<int>(ceil(color.b * 255.0f)), 0, 255);
  }
  if (with_head)
    ss << "#";
  ss << std::hex << (r << 16 | g << 8 | b);
  std::string temp = ss.str();
  return ss.str();
}
std::string rgb_to_string(glm::vec3 color) {
  int r, g, b;
  if (color.r < 0.5f) {
    r = glm::clamp(static_cast<int>(floor(color.r * 255.0f)), 0, 255);
  } else {
    r = glm::clamp(static_cast<int>(ceil(color.r * 255.0f)), 0, 255);
  }
  if (color.g < 0.5f) {
    g = glm::clamp(static_cast<int>(floor(color.g * 255.0f)), 0, 255);
  } else {
    g = glm::clamp(static_cast<int>(ceil(color.g * 255.0f)), 0, 255);
  }
  if (color.b < 0.5f) {
    b = glm::clamp(static_cast<int>(floor(color.b * 255.0f)), 0, 255);
  } else {
    b = glm::clamp(static_cast<int>(ceil(color.b * 255.0f)), 0, 255);
  }
  std::string str{};
  str +=
      std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b);
  return str;
}
glm::vec2 get_rect_center(Rect rect) {
  float x = (rect.right() + rect.left()) / 2.0f;
  float y = (rect.top() + rect.bottom()) / 2.0f;
  return {x, y};
}
} // namespace util

} // namespace mare