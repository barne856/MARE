#ifndef SPOTLIGHT
#define SPOTLIGHT

// MARE
#include "mare/Entities/Camera.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
struct light_properties {
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
};
class Spotlight : public Camera {
public:
  Spotlight() : Camera(ProjectionType::PERSPECTIVE), properties({glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f)}) {}
  light_properties properties;
};
} // namespace mare

#endif