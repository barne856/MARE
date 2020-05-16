#ifndef EULERMETHOD
#define EULERMETHOD

// MARE
#include "mare/Components/Rigidbody.hpp"
#include "mare/Systems.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {

class EulerMethod : public PhysicsSystem<Rigidbody> {
public:
  void update(float dt, Rigidbody *rb) override {
    // euler method to calculate velocity and position
    rb->linear_velocity += rb->force * dt;
    rb->translate(rb->linear_velocity * dt);
  }
};
} // namespace mare

#endif