#ifndef RIGIDBODY
#define RIGIDBODY

// MARE
#include "mare/Components/Transform.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{

class Rigidbody : virtual public Transform
{
public:
    glm::vec3 linear_velocity{};
    glm::vec3 angular_velocity{};
    glm::vec3 force{};
    glm::vec3 torque{};
};
} // namespace mare

#endif