#ifndef RIGIDBODY
#define RIGIDBODY

// MARE
#include "mare/Systems.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{

class Rigidbody : public IPhysicsSystem
{
public:
    Rigidbody() {}
    ~Rigidbody() {}
    void update(Entity *entity, float delta_time) override
    {
        // basic integrator for position
        linear_velocity += force * delta_time;
        entity->translate(linear_velocity * delta_time);
    }
    glm::vec3 linear_velocity{};
    glm::vec3 angular_velocity{};
    glm::vec3 force{};
    glm::vec3 torque{};
};
} // namespace mare

#endif