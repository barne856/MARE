#ifndef MARE
#define MARE

// Standard Library
#include <memory>
#include <variant>
#include <vector>
// External Libraries
#include "glm.hpp"

/**
 * @brief The namespace for the engine.
 * 
 */
namespace mare {
/**
 * @brief Abstract base class for all Components.
 * @details Components are used to add functionality to Entities through data
 * and function members. When creating Entities, inherit from Components to add
 * the required functionality. Components are operated on by Systems.
 * @see System
 * @see Entity
 */
class Component {
public:
  virtual ~Component() = 0;
};
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
 * @brief The namespace for utility methods and variables in the engine.
 * 
 */
namespace util {} // namespace util

/**
 * @brief The namespace for math methods and varibles in the engine.
 * 
 */
namespace math {
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

/** The ratio of a circle's circumference to its diameter approximated as a
 * 32-bit floating point number.*/
const float PI = 3.141592653f;
} // namespace math

} // namespace mare

#endif