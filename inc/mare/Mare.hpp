#ifndef MARE
#define MARE

// Standard Library
#include <algorithm>
#include <memory>
#include <variant>
#include <vector>
#include <string>

// External Libraries
#include "glm.hpp"

/**
 * @brief The namespace for the engine.
 *
 */
namespace mare {
/**
 * @brief Abstract base class for all Systems.
 * @details Systems provide callbacks that operate on Components. There are
 * three main types of systems:
 * - Rendering Systems
 * - Physics Systems
 * - Control Systems
 *
 * Each Entity contains a *system stack* which references of all the
 * Systems enabled on that Entity. The ordering of the Systems on the system
 * stack determines the order in which the callbacks are executed. Rendering and
 * Physics Systems are executed front to back while Controls Systems are
 * executed in reverse order.
 * @see Entity
 * @see IRenderSystem
 * @see IPhysicsSystem
 * @see IControlsSystem
 */
class System {
public:
  virtual ~System() = 0;
};

/**
 * @brief A Scoped variable is a reference to a variable that has a single
 * owner which is responsible for the lifetime of the variable and cannot be
 * shared between scopes. However, it can be moved between scopes using
 * `std::move`. Once the owner of the Scoped variable is destroyed, the Scoped
 * variable is also destroyed.
 *
 * @tparam <T> The type of the Scoped variable.
 */
template <typename T> using Scoped = std::unique_ptr<T>;
/**
 * @brief A Referenced variable is a reference to a variable that can have one
 * or more owners that are responsible for the lifetime of the variable. Once
 * all owners of the Referenced variable are destroyed, the Referenced variable
 * is destroyed.
 *
 * @tparam <T> The type of the Referenced variable.
 */
template <typename T> using Referenced = std::shared_ptr<T>;

/**
 * @brief Used to generate a Scoped variable.
 *
 * @tparam <T> The type of Scoped variable to generate
 * @tparam <Args> The types of the arguments used in the constructor of the
 * variable.
 * @param args The arguments used in the constructor of the variable.
 * @return The Scoped variable.
 */
template <typename T, typename... Args> Scoped<T> gen_scoped(Args... args) {
  return std::make_unique<T>(args...);
}
/**
 * @brief Used to generate a Referenced variable.
 *
 * @tparam <T> The type of Referenced variable to generate
 * @tparam <Args> The types of the arguments used in the constructor of the
 * variable.
 * @param args The arguments used in the constructor of the variable.
 * @return The Referenced variable.
 */
template <typename T, typename... Args> Referenced<T> gen_ref(Args... args) {
  return std::make_shared<T>(args...);
}

/**
 * @brief The namespace for math methods and varibles in the engine.
 *
 */
namespace math {
  /**
   * @brief Calculate the angle in radians between two 3D vectors
   * 
   * @param v1 The first vector.
   * @param v2 The second vector.
   * @return The angle between the two vectors.
   */
float angle_between(glm::vec3 v1, glm::vec3 v2);
/**
 * @brief The shoelace method for finding the area of a 2D polygon
 *
 * @param points The vertices of the polygon. If the points are provided
 * sequentially in the counterclockwise direction, the returned area will be
 * positive. If the points are provided sequentially in the clockwise direction,
 * the returned area will be negative.
 * @return The area of the polygon descibed by \p points.
 */
float shoelace(std::vector<glm::vec2> points);
/**
 * @brief Convert window coordinates to normalized device coordinates.
 * @details Window coordinates are measured in pixels from the top left corner
 * of the window. Normalized device coordinates are measured across the window
 * from left to right and bottom to top and range from -1 to 1.
 *
 * @param[in] x The x-coordinate in window coordinates.
 * @param[in] y The y-coordinate in window coordinates.
 * @param[out] nx The coressponding x-coordinate in normalized device
 * coordinates.
 * @param[out] ny The coressponding y-coordinate in normalized device
 * coordinates.
 */
void normalized_device_coordinates(int x, int y, float &nx, float &ny);
/**
 * @brief Convert normalized device coordinates to window coordinates.
 * @details Window coordinates are measured in pixels from the top left corner
 * of the window. Normalized device coordinates are measured across the window
 * from left to right and bottom to top and range from -1 to 1.
 *
 * @param[in] nx The x-coordinate in normalized device coordinates.
 * @param[in] ny The y-coordinate in normalized device coordinates.
 * @param[out] x The coressponding x-coordinate in window coordinates.
 * @param[out] y The coressponding y-coordinate in window coordinates.
 */
void window_coordinates(float nx, float ny, int &x, int &y);
/**
 * @brief Return true if \p point is inside \p polygon.
 * @details \p polygon vertices must be given in counter-clockwise order.
 *
 * @param point The point to test.
 * @param polygon The polygon to test.
 * @return true
 * @return false
 */
bool is_in_polygon(glm::vec2 point, std::vector<glm::vec2> polygon);
/**
 * @brief Returns the closest point on a line segment to the given point \p
 * point.
 *
 * @param point The point to snap
 * @param p1 First point of line segment to snap to.
 * @param p2 Second point of line segment to snap to.
 * @return The snapped point.
 */
glm::vec2 snap_to_line(glm::vec2 point, glm::vec2 p1, glm::vec2 p2);
/**
 * @brief Return the intersection point of two lines
 *
 * @param p1 point 1 on line 1
 * @param p2 point 2 on line 1
 * @param p3 point 1 on line 2
 * @param p4 point 2 on line 2
 * @return The intersection point, nan if the lines are parallel or either line
 * is degenerate
 */
glm::vec2 intersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
/**
 * @brief Calculate the closest point that lies inside of a triangle to another
 * point.
 * @details The triangle vertices should be provided in counter-clockwise order
 * starting with \p v1.
 *
 * @param v1 Triangle vertex 1.
 * @param v2 Triangle vertex 2.
 * @param v3 Triangle vertex 3.
 * @param point The point to test.
 * @return The closest point inside the triangle to \p point.
 */
glm::vec2 clamp_point_to_triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3,
                                  glm::vec2 point);
/** The ratio of a circle's circumference to its diameter approximated as a
 * 32-bit floating point number.*/
const float PI = 3.141592653f;
/** Two times PI*/
const float TAU = 2.0f * 3.141592653f;
} // namespace math

/**
 * @brief The namespace for utility methods and variables in the engine.
 *
 */
namespace util {
/**
 * @brief Convert rgba colors to hsva colors
 * @details rgba values range from 0 to 1, h value ranges from 0 to 2*pi, s
 * and v range from 0 to 1.
 *
 * @param color The RGBA color
 * @return The HSVA color
 */
glm::vec4 rgba_to_hsva(glm::vec4 color);
/**
 * @brief Convert hsva colors to rgba colors
 * @details rgba values range from 0 to 1, h value ranges from 0 to 2*pi, s
 * and v range from 0 to 1.
 *
 * @param color The HSVA color
 * @return The RGBA color
 */
glm::vec4 hsva_to_rgba(glm::vec4 color);
/**
 * @brief Convert an rgb color to a hex string.
 *
 * @param color The rgb color
 * @param with_head Return a string with the "#" in front.
 * @return The hex string
 */
std::string rgb_to_hex(glm::vec3 color, bool with_head = false);
/**
 * @brief Convert an rgb color to a string
 * @details Returns a comma separated string with r,g,b values ranging from
 * 0-255.
 *
 * @param color The RGB color
 * @return The formatted string
 */
std::string rgb_to_string(glm::vec3 color);
/**
 * @brief A simple struct to define a bounding box of a UI element.
 */
struct Rect {
  float &left() { return m_values[0]; }
  float &bottom() { return m_values[1]; }
  float &right() { return m_values[2]; }
  float &top() { return m_values[3]; }

  void set_rect(glm::vec2 bottom_left, glm::vec2 top_right) {
    m_values = glm::vec4(bottom_left, top_right);
  }

  float operator[](unsigned i) const { return m_values[i]; }
  float &operator[](unsigned i) { return m_values[i]; }

private:
  glm::vec4 m_values{};
};
/**
 * @brief Get the center of a Rect
 * 
 * @param rect The Rect
 * @return The center of the Rect
 */
glm::vec2 get_rect_center(Rect rect);
} // namespace util

} // namespace mare

#endif