#ifndef SPOTLIGHT
#define SPOTLIGHT

// MARE
#include "mare/Entities/Camera.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief Properties of a basic Phong-style light.
 */
struct light_properties {
  glm::vec4 ambient; /**< Ambient color that from the environment. Constant and
                        not dependent on any amount of light.*/
  glm::vec4 diffuse; /**< Directional component from the light.*/
  glm::vec4
      specular; /**< Directional depending on the reflection off the surface.*/
};
/**
 * @brief A simple Spotlight class
 * @details A Spotlight inherits from a Camera so that it can have a view and a
 * position.
 */
class Spotlight : public Camera {
public:
  /**
   * @brief Construct a new Spotlight object
   */
  Spotlight()
      : Camera(ProjectionType::PERSPECTIVE),
        properties({glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f)}) {}
  light_properties properties; /**< The properties of the Spotlight.*/
};
} // namespace mare

#endif