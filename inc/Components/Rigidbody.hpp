#ifndef RIGIDBODY
#define RIGIDBODY

// MARE
#include "Entity.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A Rigidbody Component that provides Rigidbody properties.
 */
class Rigidbody : virtual public Entity {
public:
  glm::vec3 linear_velocity{};  /**< The linear velocity of the body.*/
  glm::vec3 angular_velocity{}; /**< The angular velocity of the body.*/
  glm::vec3 force{};            /**< The linear force acting on the body.*/
  glm::vec3 torque{};           /**< The angular force acting on the body.*/
};
} // namespace mare

#endif